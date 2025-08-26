#include <iostream>

#include "ast.hpp"
#include "llvm_codegen.hpp"
#include "marbl.hpp"
#include "parser.hpp"
#include "printer.hpp"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Host.h"
#include <llvm/IR/LegacyPassManager.h>

int compile(std::vector<UniqueStmt> &statements, std::string filename) {
    for (auto &statement : statements) {
        AstPrinter printer{};
        printer.print(*statement);

        // Interpreter interpreter{};
        // interpreter.interpret(*statement);
    }

    // Generate IR
    CodeGenVisitor codegen(filename);
    codegen.generate(statements);

    // Print IR to stdout
    std::cout << "Generated LLVM IR:\n";
    codegen.getModule().print(llvm::outs(), nullptr);

    // Initialize LLVM targets
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    // Create target machine
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    std::string error;
    auto *target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

    if (!target) {
        llvm::errs() << "Failed to lookup target: " << error << "\n";
        return 1;
    }

    llvm::TargetOptions opt;
    auto RM = std::optional<llvm::Reloc::Model>(llvm::Reloc::PIC_);
    std::unique_ptr<llvm::TargetMachine> targetMachine(
        target->createTargetMachine(targetTriple, "generic", "", opt, RM));

    codegen.getModule().setDataLayout(targetMachine->createDataLayout());
    codegen.getModule().setTargetTriple(targetTriple);

    // Emit object file
    std::error_code EC;
    llvm::raw_fd_ostream dest("build/output.o", EC, llvm::sys::fs::OF_None);
    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message() << "\n";
        return 1;
    }

    llvm::legacy::PassManager pass;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile)) {
        llvm::errs() << "TargetMachine can't emit object file\n";
        return 1;
    }

    pass.run(codegen.getModule());
    dest.flush();

    std::cout << "Object file 'build/output.o' generated successfully!\n";
    return 0;
}

// We follow the conventions defined in UNIX "sysexits.h" header for exit codes:
// (https://man.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html).
int main(int argc, char **argv) {
    // if (argc > 2) {
    //     std::cerr << "Usage: marbl [script]" << std::endl;
    //     return EX_USAGE;
    // }

    // if (argc == 2) { return Marbl::runFile(argv[1]); }

    // return Marbl::runPrompt();

    std::ifstream inputFile(argv[1]);

    if (!inputFile) {
        std::cerr << "Cannot open input file!" << std::endl;
        return EX_NOINPUT;
    }

    Parser parser{inputFile, std::string(argv[1])};
    std::vector<UniqueStmt> statements = parser.parse();

    inputFile.close();

    return compile(statements, argv[1]);
}
