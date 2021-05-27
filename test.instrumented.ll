; ModuleID = 'test.ll'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@0 = private unnamed_addr constant [6 x i8] c"entry\00", align 1
@1 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"sge\00", align 1
@3 = private unnamed_addr constant [14 x i8] c"land.lhs.true\00", align 1
@4 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"sgt\00", align 1
@6 = private unnamed_addr constant [7 x i8] c"if.end\00", align 1
@7 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@8 = private unnamed_addr constant [4 x i8] c"sge\00", align 1
@9 = private unnamed_addr constant [15 x i8] c"land.lhs.true3\00", align 1
@10 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@11 = private unnamed_addr constant [4 x i8] c"sle\00", align 1
@12 = private unnamed_addr constant [14 x i8] c"lor.lhs.false\00", align 1
@13 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@14 = private unnamed_addr constant [4 x i8] c"sgt\00", align 1
@15 = private unnamed_addr constant [15 x i8] c"land.lhs.true6\00", align 1
@16 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@17 = private unnamed_addr constant [4 x i8] c"slt\00", align 1
@18 = private unnamed_addr constant [6 x i8] c"entry\00", align 1
@19 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@20 = private unnamed_addr constant [14 x i8] c"land.lhs.true\00", align 1
@21 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@22 = private unnamed_addr constant [8 x i8] c"if.then\00", align 1
@23 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@24 = private unnamed_addr constant [7 x i8] c"if.end\00", align 1
@25 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@26 = private unnamed_addr constant [15 x i8] c"land.lhs.true3\00", align 1
@27 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@28 = private unnamed_addr constant [14 x i8] c"lor.lhs.false\00", align 1
@29 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@30 = private unnamed_addr constant [15 x i8] c"land.lhs.true6\00", align 1
@31 = private unnamed_addr constant [5 x i8] c"func\00", align 1
@32 = private unnamed_addr constant [9 x i8] c"if.then8\00", align 1
@33 = private unnamed_addr constant [5 x i8] c"func\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @func(i32 %a, i32 %b) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  store i32 %0, i32* %c, align 4
  %1 = load i32, i32* %a.addr, align 4
  %cmp = icmp sge i32 %1, 1
  call void @updateCondTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %1, i32 1)
  br i1 %cmp, label %land.lhs.true, label %if.end

land.lhs.true:                                    ; preds = %entry
  %2 = load i32, i32* %b.addr, align 4
  %3 = load i32, i32* %a.addr, align 4
  %cmp1 = icmp sgt i32 %2, %3
  call void @updateCondTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @4, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @3, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @5, i32 0, i32 0), i32 %2, i32 %3)
  br i1 %cmp1, label %if.then, label %if.end

if.then:                                          ; preds = %land.lhs.true
  %4 = load i32, i32* %c, align 4
  %add = add nsw i32 %4, 1
  store i32 %add, i32* %c, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %land.lhs.true, %entry
  call void @updateNameTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @19, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @18, i32 0, i32 0))
  call void @updateNameTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @21, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @20, i32 0, i32 0))
  call void @updateNameTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @23, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @22, i32 0, i32 0))
  %5 = load i32, i32* %a.addr, align 4
  %cmp2 = icmp sge i32 %5, 1
  call void @updateCondTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @7, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @6, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @8, i32 0, i32 0), i32 %5, i32 1)
  br i1 %cmp2, label %land.lhs.true3, label %lor.lhs.false

land.lhs.true3:                                   ; preds = %if.end
  %6 = load i32, i32* %c, align 4
  %cmp4 = icmp sle i32 %6, 2
  call void @updateCondTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @10, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @9, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @11, i32 0, i32 0), i32 %6, i32 2)
  br i1 %cmp4, label %if.then8, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %land.lhs.true3, %if.end
  %7 = load i32, i32* %b.addr, align 4
  %cmp5 = icmp sgt i32 %7, 3
  call void @updateCondTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @13, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @12, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @14, i32 0, i32 0), i32 %7, i32 3)
  br i1 %cmp5, label %land.lhs.true6, label %if.end9

land.lhs.true6:                                   ; preds = %lor.lhs.false
  %8 = load i32, i32* %b.addr, align 4
  %cmp7 = icmp slt i32 %8, 5
  call void @updateCondTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @16, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @15, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @17, i32 0, i32 0), i32 %8, i32 5)
  br i1 %cmp7, label %if.then8, label %if.end9

if.then8:                                         ; preds = %land.lhs.true6, %land.lhs.true3
  %9 = load i32, i32* %c, align 4
  store i32 %9, i32* %a.addr, align 4
  br label %if.end9

if.end9:                                          ; preds = %if.then8, %land.lhs.true6, %lor.lhs.false
  call void @updateNameTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @25, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @24, i32 0, i32 0))
  call void @updateNameTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @27, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @26, i32 0, i32 0))
  call void @updateNameTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @29, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @28, i32 0, i32 0))
  call void @updateNameTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @31, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @30, i32 0, i32 0))
  call void @updateNameTable(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @33, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @32, i32 0, i32 0))
  %10 = load i32, i32* %a.addr, align 4
  call void @getTotalConditions(i32 8)
  call void @getScanSig()
  ret i32 %10
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @output(i8* %s) #0 {
entry:
  %s.addr = alloca i8*, align 8
  store i8* %s, i8** %s.addr, align 8
  %0 = load i8*, i8** %s.addr, align 8
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i8* %0)
  call void @getTotalConditions(i32 8)
  ret void
}

declare dso_local i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  call void @mainInit()
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 0, i32* %a, align 4
  store i32 1, i32* %b, align 4
  %0 = load i32, i32* %a, align 4
  %1 = load i32, i32* %b, align 4
  %call = call i32 @func(i32 %0, i32 %1)
  store i32 1, i32* %a, align 4
  store i32 1, i32* %b, align 4
  %2 = load i32, i32* %a, align 4
  %3 = load i32, i32* %b, align 4
  %call1 = call i32 @func(i32 %2, i32 %3)
  store i32 2, i32* %a, align 4
  store i32 3, i32* %b, align 4
  %4 = load i32, i32* %a, align 4
  %5 = load i32, i32* %b, align 4
  %call2 = call i32 @func(i32 %4, i32 %5)
  store i32 2, i32* %a, align 4
  store i32 4, i32* %b, align 4
  %6 = load i32, i32* %a, align 4
  %7 = load i32, i32* %b, align 4
  %call3 = call i32 @func(i32 %6, i32 %7)
  ret i32 0
}

declare void @mainInit()

declare void @updateNameTable(i8*, i8*)

declare void @updateCondTable(i8*, i8*, i8*, i32, i32)

declare void @getTotalConditions(i32)

declare void @getScanSig()

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0 "}
