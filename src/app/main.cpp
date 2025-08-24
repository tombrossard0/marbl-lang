#include <iostream>

#include "ast.hpp"
#include "llvm_codegen.hpp"
#include "marbl.hpp"

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

// We follow the conventions defined in UNIX "sysexits.h" header for exit codes:
// (https://man.freebsd.org/cgi/man.cgi?query=sysexits&apropos=0&sektion=0&manpath=FreeBSD+4.3-RELEASE&format=html).
int main(int argc, char **argv) {
    // if (argc > 2) {
    //     std::cerr << "Usage: marbl [script]" << std::endl;
    //     return EX_USAGE;
    // }

    // if (argc == 2) { return Marbl::runFile(argv[1]); }

    // return Marbl::runPrompt();

    // Example: (10 + 20) * 2
    auto expr = std::make_unique<Binary>(
        std::make_unique<Binary>(std::make_unique<Literal>(10), Token{TokenType::PLUS, "+", "+", 1, 1},
                                 std::make_unique<Literal>(20)),
        Token{TokenType::STAR, "*", "*", 1, 1}, std::make_unique<Literal>(2));

    // Generate IR
    CodeGenVisitor codegen("marbl");
    codegen.generate(*expr);

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
    auto RM = std::optional<llvm::Reloc::Model>();
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
