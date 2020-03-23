# functionObjects

## field/LOL

运行后计算火焰浮起长度（Lift-off Length）
controlDict 中添加：

```
functions
{
    LOL
    {
        type LOL;
        libs ("libZYfieldFunctionObjects.so");
        OH_max 0.0001;
    }
}
```

```
postProcess -latestTime -field OH
```

## utilities/engineTimeActivatedFileUpdate

OpenFOAM 自带的 timeActivatedFileUpdate 不支持发动机算例。
因为在算例里边，发动机里边的 Time 是按照曲轴转角来计算的。
但是在代码中仍然是按照秒来计算的。

经过我的修改后，在 controlDict 中加入：
```
functions
{
    fileUpdate1
    {
        type              engineTimeActivatedFileUpdate;
        libs              ("libZYutilityFunctionObjects.so");
        writeControl      timeStep;
        writeInterval     1;
        fileToUpdate      "$FOAM_CASE/system/controlDict";
        timeVsFile
        (
            (-179 "$FOAM_CASE/system/controlDict1")
            (-175 "$FOAM_CASE/system/controlDict2")
        );
    }
}
```

然后复制几份 controlDict

```
cp system/controlDict system/controlDict1
cp system/controlDict system/controlDict2
```