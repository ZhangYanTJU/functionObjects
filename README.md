# functionObjects

## field/LOL

calulate Lift-off Length after the simulation
controlDict:

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
postProcess -field OH -func LOL
```

## utilities/engineTimeActivatedFileUpdate

The official timeActivatedFileUpdate doesn't support for the engine simulation, because the time in engine simulation is Crank Angle.

controlDict:
```
functions
{
    fileUpdate
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


```
cp system/controlDict system/controlDict1
cp system/controlDict system/controlDict2
```


## lagrangian/outputTheta

```
functions
{
    outputTheta
    {
        type outputTheta;
        libs ("libZYlagrangianFunctionObjects.so");
        clouds
        (
            sprayCloud
        );
    }
}
```


```
sprayFoam -postProcess -func outputTheta
```