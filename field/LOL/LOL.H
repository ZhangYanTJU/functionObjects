/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2018 OpenFOAM Foundation
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
    Foam::functionObjects::LOL

Description
    Calculates the natural LOLarithm of the specified scalar field.

    Performs \f$ln(max(x, a))\f$ where \f$x\f$ is the field and \f$a\f$ an
    optional clip to handle 0 or negative \f$x\f$.

See also
    Foam::functionObjects::fieldExpression
    Foam::functionObjects::fvMeshFunctionObject

SourceFiles
    LOL.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_LOL_H
#define functionObjects_LOL_H

#include "fieldExpression.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class LOL Declaration
\*---------------------------------------------------------------------------*/

class LOL
:
    public fieldExpression
{
    // Private Data
        IOdictionary sprayCloudProperties;
        vector position;
        vector direction;
        scalar OH_max;
        OFstream ResultOutPut;

    // Private Member Functions

        //- Calculate the LOL field and return true if successful
        virtual bool calc();


public:

    //- Runtime type information
    TypeName("LOL");


    // Constructors

        //- Construct from Time and dictionary
        LOL
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~LOL();


    // Member Functions

        //- Read the randomise data
        //virtual bool read(const dictionary&);

        //- currently does nothing
        virtual bool execute();
        virtual bool write();
        virtual bool clear();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //