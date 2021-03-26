/*
 * @Description: A Simple demo pass for learning llvm
 * Reference link: https://blog.csdn.net/xiaoxiaowenqiang/article/details/104873800
 * 
 * @Author: Fishermanykx
 * @Date: 2021-03-25 16:10:20
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-03-26 08:39:13
 */

#include "ProjHeaders.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.  通过 RecursiveASTVisitor 可以遍历特点的节点 并添加重写内容
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor>
{
  // RecursiveASTVisitor类负责实际对源码的改写
public:
  MyASTVisitor(Rewriter &R) : TheRewriter(R) {}

  // 在RecursiveASTVisitor中重写VisitStmt函数与VisitFunctionDecl函数实现源码中目标语素的检测以及改写动作
  // 改写好的源码送入Rewriter类中，进行写入源代码文件的动作

  // 语句statements Stmt  遍历 expression表达式
  bool VisitStmt(Stmt *s)
  {
    // Only care about If statements.
    if (isa<IfStmt>(s))
    { // 是 if 语句
      IfStmt *IfStatement = cast<IfStmt>(s);
      Stmt *Then = IfStatement->getThen();

      TheRewriter.InsertText(Then->getBeginLoc(), "// the 'if' part\n", true,
                             true);
      // 在if语句 后面添加注释   getLocStart() 旧接口 ---> getBeginLoc()  clang/AST/Stmt.h中

      Stmt *Else = IfStatement->getElse();
      if (Else)
        TheRewriter.InsertText(Else->getBeginLoc(), "// the 'else' part\n",
                               true, true);
      // 在else语句后面添加注释
    }

    return true;
  }

  // 函数定义(function definitions) 遍历
  bool VisitFunctionDecl(FunctionDecl *f)
  {
    // Only function definitions (with bodies), not declarations.
    if (f->hasBody())
    { // 有函数体 函数定义

      // 函数体是一个语句集和 statements set
      Stmt *FuncBody = f->getBody();

      // Type name as string
      QualType QT = f->getReturnType();       // 函数返回值类型
      std::string TypeStr = QT.getAsString(); // 对于的类型字符串

      // Function name
      DeclarationName DeclName = f->getNameInfo().getName(); // 函数名
      std::string FuncName = DeclName.getAsString();

      // param
      int param_num = f->getNumParams(); // clang/AST/Decl.h中
      std::string func_param;
      //for (FunctionDecl::param_iterator fit = f->param_begin(); fit != f->param_end(); fit++)
      for (int i = 0; i < param_num; i++)
      {
        ParmVarDecl *ptemp = f->getParamDecl(i);
        func_param += " | ";

        //func_param += fit->getOriginalType().getAsString();

        func_param += ptemp->getOriginalType().getAsString();
      }

      // Add comment before  生成函数头注释
      std::stringstream SSBefore;
      SSBefore << "// Begin function " << FuncName << ", returning " << TypeStr << ", param num: " << param_num << ", type: " << func_param
               << "\n";

      // 获取函数开头位置
      SourceLocation ST = f->getSourceRange().getBegin();
      // 插入注释
      TheRewriter.InsertText(ST, SSBefore.str(), true, true);

      // And after 添加函数尾注释
      std::stringstream SSAfter;
      SSAfter << "\n// End function " << FuncName;
      ST = FuncBody->getEndLoc().getLocWithOffset(1); // 函数体结束后的后面一个位置   getLocEnd() 旧接口 ---> getEndLoc()
      //  插入注释
      TheRewriter.InsertText(ST, SSAfter.str(), true, true);
    }

    return true;
  }

private:
  // 代码重写类对象实例
  Rewriter &TheRewriter;
};

// ASTConsumer负责读取Clang解析出来的AST树 并调用 MyASTVisitor 进行 匹配与改写
// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer
{
public:
  MyASTConsumer(Rewriter &R) : Visitor(R) {}

  // Override the method that gets called for each parsed top-level
  // declaration.   遍历声明
  // 在ASTConsumer中重写HandleTopLevelDecl函数用以检测源码中的函数声明语句
  bool HandleTopLevelDecl(DeclGroupRef DR) override
  {
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b)
    {
      // Traverse the declaration using our AST visitor.

      // 逐个遍历 调用 MyASTVisitor 对源码进行匹配与改写
      Visitor.TraverseDecl(*b);

      (*b)->dump();
    }
    return true;
  }

private:
  MyASTVisitor Visitor; // 上面定义的 遍历AST类实例
};

// 前端 动作执行 类

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction
{
public:
  MyFrontendAction() {}
  void EndSourceFileAction() override
  {
    // 源码管理器
    SourceManager &SM = TheRewriter.getSourceMgr();
    llvm::errs() << "** EndSourceFileAction for: "
                 << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n"; // 打印源码文件名

    // Now emit the rewritten buffer.
    TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override
  {
    llvm::errs() << "** Creating AST consumer for: " << file << "\n";
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MyASTConsumer>(TheRewriter);
  }

private:
  Rewriter TheRewriter;
};

int main(int argc, const char **argv)
{
  CommonOptionsParser op(argc, argv, ToolingSampleCategory);
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());

  // ClangTool::run accepts a FrontendActionFactory, which is then used to
  // create new objects implementing the FrontendAction interface. Here we use
  // the helper newFrontendActionFactory to create a default factory that will
  // return a new MyFrontendAction object every time.
  // To further customize this, we could create our own factory class.
  return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}