# functionObjects

[![OpenFOAM version](https://img.shields.io/badge/OpenFOAM-7-brightgreen)](https://github.com/OpenFOAM/OpenFOAM-7)
[![OpenFOAM version](https://img.shields.io/badge/OpenFOAM-8-brightgreen)](https://github.com/OpenFOAM/OpenFOAM-8)

## field/LOL

calulate Lift-off Length after the simulation
system/LOL:

```c++
FoamFile
{
    version         2.0;
    format          ascii;
    class           dictionary;
    location        system;
    object          LOL;
}

enabled         true;
type            LOL;
libs            ("libZYfieldFunctionObjects.so");

field OH;
//field OHMean; // for LES
```

### for RANS
```
foamDictionary -entry field -set OH system/LOL
postProcess -field OH -func LOL
```
### for LES
add OH average during CFD online:
```
functions
{
	fieldAverage
    {
        type            fieldAverage;
        functionObjectLibs ("libfieldFunctionObjects.so");
        enabled         true;

        // Time at which averaging should start
		timeStart	0.0012;

        writeControl   outputTime;

        fields
        (
            OH 
            {
                mean        on;
                prime2Mean  off;
                base        time;
            }
        );
    }	
}
```
after finish CFD:
```
foamDictionary -entry field -set OHMean system/LOL
postProcess -field OHMean -func LOL
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

## utilities/setWriteInterval

change writeInterval for the given table, the unit is CA

for OpenFOAM-7 and OpenFOAM-8, you need to change the official code a little.
add a new function in Time.H (after setDeltaTNoAdjust function):
```
//- Reset the write interval
virtual void setWriteInterval(const scalar writeInterval);
```

add its definition in Time.C
```
void Foam::Time::setWriteInterval(const scalar writeInterval)
{
	writeInterval_ = writeInterval;

	if
	(
		writeControl_ == writeControl::runTime
	 || writeControl_ == writeControl::adjustableRunTime
	)
	{
		// Recalculate writeTimeIndex_ for consistency with the new
		// writeInterval
		writeTimeIndex_ = label
		(
			((value() - startTime_) + 0.5*deltaT_)/writeInterval_
		);
	}
	else if (writeControl_ == writeControl::timeStep)
	{
		// Set to the nearest integer
		writeInterval_ = label(writeInterval + 0.5);
	}
}
```
in TimeIO.C
comment these lines:
```
scalar oldWriteInterval = writeInterval_;

if (oldWriteInterval != writeInterval_)
{
    switch (writeControl_)
    {
        case writeControl::runTime:
        case writeControl::adjustableRunTime:
            // Recalculate writeTimeIndex_ to be in units of current
            // writeInterval.
            writeTimeIndex_ = label
            (
                writeTimeIndex_
                * oldWriteInterval
                / writeInterval_
            );
        break;

        default:
        break;
    }
}
```

in TimeIO.C, function `Foam::Time::read()`, add a function call after `readDict();`:
```
setWriteInterval(writeInterval_);
```
in TimeIO.C, function `Foam::Time::readModifiedObjects()`, change `readDict();` to:
```
scalar oldWriteInterval = writeInterval_;
readDict();
if (!equal(oldWriteInterval, writeInterval_))
{
    setWriteInterval(writeInterval_);
}
```

controlDict:
```
functions
{
    setWriteInterval
    {
        type setWriteInterval;
        libs ("libZYutilityFunctionObjects.so");


        writeInterval table // linear interpolation
        (
            (-140      0.1)
            (-139      0.1)
            (-138.99999999999      5)
            (-130      5)
            (-129      6) // keep 6 forever, will not extrapolate
        );
    }
}
```


## utilities/setEngineWriteInterval

hard code to change writeInterval:
```
if (CA < SOI_ - 1) setWriteInterval(10.);
if (CA > SOI_ - 1) setWriteInterval(1.);
if (CA > SOI_ + 20) setWriteInterval(2.);
if (CA > -10) setWriteInterval(0.25); // if conflict with last term, overload it
if (CA > 20) setWriteInterval(10.); // after 20 CA, writeInterval will be 10 CA forever
```

controlDict:
```
functions
{
    setEngineWriteInterval
    {
        type setEngineWriteInterval;
        libs ("libZYutilityFunctionObjects.so");
    }
}
```


```
cp system/controlDict system/controlDict1
cp system/controlDict system/controlDict2
```


## lagrangian/outputTheta

Write the particle volume fraction field to time directory.

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
