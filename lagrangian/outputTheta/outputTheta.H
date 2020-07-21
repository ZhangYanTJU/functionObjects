/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
July 2019: Bruno Santos @ FSD blueCAPE Lda
           - created function object based on OpenFOAM's cloudInfo
-------------------------------------------------------------------------------
License
    This file is derivative work of OpenFOAM.

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
    Foam::functionObjects::outputTheta

Group
    grpLagrangianFunctionObjects

Description
    Writes Lagrangian cloud's positions to the old format, into the file
    'positions.orig'.

    Example of function object specification:
    \verbatim
    outputTheta1
    {
    outputTheta1
    {
        type        outputTheta;
        libs        ("liblagrangianExtraFunctionObjects.so");

        writeControl writeTime;

        clouds
        (
            kinematicCloud
        );
    }
    }
    \endverbatim


Usage
    \table
        Property     | Description             | Required    | Default value
        type         | type name: outputTheta    | yes         |
        clouds       | list of clouds names to process |yes  |
    \endtable

    The output data of each cloud is written to a file named \<cloudName\>.dat

See also
    Foam::functionObject
    Foam::functionObjects::regionFunctionObject

SourceFiles
    outputTheta.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_outputTheta_H
#define functionObjects_outputTheta_H

#include "regionFunctionObject.H"
#include "logFiles.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class outputTheta Declaration
\*---------------------------------------------------------------------------*/

class outputTheta
:
    public regionFunctionObject
{
protected:

    // Protected Member Functions


private:

    // Private data

        //- Cloud names
        wordList names;

    // Private member functions

        //- Disallow default bitwise copy construct
        outputTheta(const outputTheta&);

        //- Disallow default bitwise assignment
        void operator=(const outputTheta&);


public:

    //- Runtime type information
    TypeName("outputTheta");


    // Constructors

        //- Construct from Time and dictionary
        outputTheta
        (
            const word& name,
            const Time& runTime,
            const dictionary&
        );


    //- Destructor
    virtual ~outputTheta();


    // Member Functions

        //- Read the controls
        virtual bool read(const dictionary&);

        //- Execute, currently does nothing
        virtual bool execute();

        //- Write
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
