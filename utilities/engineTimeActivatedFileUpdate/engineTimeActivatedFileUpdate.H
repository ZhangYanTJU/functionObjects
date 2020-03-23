/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
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
    Foam::functionObjects::engineTimeActivatedFileUpdate

Description
    Performs a file copy/replacement once a specified time has been reached.

    Example usage to update the fvSolution dictionary at various times
    throughout the calculation:

    \verbatim
    fileUpdate1
    {
        type              engineTimeActivatedFileUpdate;
        libs              ("libutilityFunctionObjects.so");
        writeControl      timeStep;
        writeInterval     1;
        fileToUpdate      "$FOAM_CASE/system/fvSolution";
        timeVsFile
        (
            (-1 "$FOAM_CASE/system/fvSolution.0")
            (0.10 "$FOAM_CASE/system/fvSolution.10")
            (0.20 "$FOAM_CASE/system/fvSolution.20")
            (0.35 "$FOAM_CASE/system/fvSolution.35")
        );
    }
    \endverbatim

SourceFiles
    engineTimeActivatedFileUpdate.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_engineTimeActivatedFileUpdate_H
#define functionObjects_engineTimeActivatedFileUpdate_H

#include "functionObject.H"
#include "Tuple2.H"
#include "dimensionedScalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Time;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                  Class engineTimeActivatedFileUpdate Declaration
\*---------------------------------------------------------------------------*/

class engineTimeActivatedFileUpdate
:
    public functionObject
{
    // Private Data

        //- Reference to Time
        const Time& time_;

        dimensionedScalar rpm_;

        //- Name of file to update
        fileName fileToUpdate_;

        //- List of times vs filenames
        List<Tuple2<scalar, fileName>> timeVsFile_;

        //- Index of last file copied
        label lastIndex_;


    // Private Member Functions

        //- Update file
        void updateFile();

        scalar timeToDeg(const scalar t);

        //- Disallow default bitwise copy construction
        engineTimeActivatedFileUpdate(const engineTimeActivatedFileUpdate&) = delete;

        //- Disallow default bitwise assignment
        void operator=(const engineTimeActivatedFileUpdate&) = delete;


public:

    //- Runtime type information
    TypeName("engineTimeActivatedFileUpdate");


    // Constructors

        //- Construct from Time and dictionary
        engineTimeActivatedFileUpdate
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~engineTimeActivatedFileUpdate();


    // Member Functions

        //- Read the engineTimeActivatedFileUpdate data
        virtual bool read(const dictionary&);

        //- Execute file updates
        virtual bool execute();

        //- Do nothing
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
