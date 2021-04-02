/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2020 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.
    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.
    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
Class
    Foam::functionObjects::setEngineWriteIntervalFunctionObject
Description
    Updates the writeInterval as a Function1 of time.
    Examples of function object specification:
    \verbatim
    setEngineWriteInterval
    {
        type setEngineWriteInterval;
        libs ("libutilityFunctionObjects.so");
        writeInterval table
        (
            (0      0.005)
            (0.1    0.005)
            (0.1001 0.01)
            (0.2    0.01)
            (0.2001 0.02)
        );
    }
    \endverbatim
    will cause results to be written every 0.005s between 0 and 0.1s, every
    0.01s between 0.1 and 0.2s and every 0.02s thereafter.
SourceFiles
    setEngineWriteIntervalFunctionObject.C
\*---------------------------------------------------------------------------*/

#ifndef functionObjects_setEngineWriteIntervalFunctionObject_H
#define functionObjects_setEngineWriteIntervalFunctionObject_H

#include "functionObject.H"
#include "Function1.H"
#include "Time.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                  Class setEngineWriteIntervalFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class setEngineWriteIntervalFunctionObject
:
    public functionObject
{
    // Private Data

        //- Reference to the time database
        const Time& time_;

        scalar SOI_;


public:

    //- Runtime type information
    TypeName("setEngineWriteInterval");


    // Constructors

        //- Construct from components
        setEngineWriteIntervalFunctionObject
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- Disallow default bitwise copy construction
        setEngineWriteIntervalFunctionObject
        (
            const setEngineWriteIntervalFunctionObject&
        ) = delete;


    // Destructor
    virtual ~setEngineWriteIntervalFunctionObject();


    // Member Functions

        //- Read and reset the writeInterval Function1
        virtual bool read(const dictionary&);

        //- Reset the writeInterval from the Function1 of time
        virtual bool execute();

        //- Do nothing
        virtual bool write();


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const setEngineWriteIntervalFunctionObject&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
