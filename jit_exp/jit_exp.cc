#include <iostream>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/MCJIT.h>

typedef double (*type_func)(double, double);

int main(void)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    llvm::SMDiagnostic smd;
    llvm::LLVMContext context;
    auto module = llvm::parseIRFile("./add.ll", smd, context);

    llvm::outs() << *module;

    auto tt = llvm::sys::getDefaultTargetTriple();

    llvm::outs() << tt << "\n";

    std::string err;
    llvm::EngineBuilder builder(std::move(module));
    auto ee = builder.setErrorStr(&err)
                     .setEngineKind(llvm::EngineKind::JIT)
                     .create(builder.selectTarget(llvm::Triple(tt), "", "", llvm::SmallVector<std::string, 4>()));
    llvm::outs() << err << "\n";
    llvm::outs() << ee << "\n";
    ee->setVerifyModules(true);
    ee->finalizeObject();

    auto addr = ee->getFunctionAddress("fpadd");
    type_func fpadd = (type_func)addr;
    auto res = fpadd(1, 2);
    std::cout << res << std::endl;
    return 0;
}
