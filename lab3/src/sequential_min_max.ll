; ModuleID = 'sequential_min_max.c'
source_filename = "sequential_min_max.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [26 x i8] c"Usage: %s seed arraysize\0A\00", align 1
@.str.3 = private unnamed_addr constant [9 x i8] c"min: %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [9 x i8] c"max: %d\0A\00", align 1
@str = private unnamed_addr constant [32 x i8] c"array_size is a positive number\00", align 1
@str.5 = private unnamed_addr constant [26 x i8] c"seed is a positive number\00", align 1

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readonly) local_unnamed_addr #0 {
  %3 = icmp eq i32 %0, 3
  br i1 %3, label %7, label %4

; <label>:4:                                      ; preds = %2
  %5 = load i8*, i8** %1, align 8, !tbaa !2
  %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str, i64 0, i64 0), i8* %5)
  br label %34

; <label>:7:                                      ; preds = %2
  %8 = getelementptr inbounds i8*, i8** %1, i64 1
  %9 = load i8*, i8** %8, align 8, !tbaa !2
  %10 = tail call i64 @strtol(i8* nocapture nonnull %9, i8** null, i32 10) #3
  %11 = trunc i64 %10 to i32
  %12 = icmp slt i32 %11, 1
  br i1 %12, label %13, label %15

; <label>:13:                                     ; preds = %7
  %14 = tail call i32 @puts(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @str.5, i64 0, i64 0))
  br label %34

; <label>:15:                                     ; preds = %7
  %16 = getelementptr inbounds i8*, i8** %1, i64 2
  %17 = load i8*, i8** %16, align 8, !tbaa !2
  %18 = tail call i64 @strtol(i8* nocapture nonnull %17, i8** null, i32 10) #3
  %19 = trunc i64 %18 to i32
  %20 = icmp slt i32 %19, 1
  br i1 %20, label %21, label %23

; <label>:21:                                     ; preds = %15
  %22 = tail call i32 @puts(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @str, i64 0, i64 0))
  br label %34

; <label>:23:                                     ; preds = %15
  %24 = shl i64 %18, 32
  %25 = ashr exact i64 %24, 30
  %26 = tail call noalias i8* @malloc(i64 %25) #3
  %27 = bitcast i8* %26 to i32*
  tail call void @GenerateArray(i32* %27, i32 %19, i32 %11) #3
  %28 = tail call i64 @GetMinMax(i32* %27, i32 0, i32 %19) #3
  %29 = trunc i64 %28 to i32
  %30 = lshr i64 %28, 32
  %31 = trunc i64 %30 to i32
  tail call void @free(i8* %26) #3
  %32 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.3, i64 0, i64 0), i32 %29)
  %33 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.4, i64 0, i64 0), i32 %31)
  br label %34

; <label>:34:                                     ; preds = %13, %23, %21, %4
  %35 = phi i32 [ 1, %4 ], [ 1, %13 ], [ 1, %21 ], [ 0, %23 ]
  ret i32 %35
}

; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) local_unnamed_addr #1

declare dso_local void @GenerateArray(i32*, i32, i32) local_unnamed_addr #2

declare dso_local i64 @GetMinMax(i32*, i32, i32) local_unnamed_addr #2

; Function Attrs: nounwind
declare dso_local void @free(i8* nocapture) local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local i64 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #1

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #3

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1 (Red Hat 8.0.1-8.el7)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"any pointer", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
