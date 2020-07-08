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

\*---------------------------------------------------------------------------*/

#include "LOL.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(LOL, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        LOL,
        dictionary
    );
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
// useless:
bool Foam::functionObjects::LOL::calc(){return true;}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::LOL::LOL
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fieldExpression(name, runTime, dict, "OH"),
    sprayCloudProperties
    (
        IOobject
        (
            "sprayCloudProperties",
            mesh_.time().constant(),
            mesh_,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        )
    ),
    position(sprayCloudProperties.subDict("subModels").subDict("injectionModels").subDict("model1").lookup("position")),
    direction(sprayCloudProperties.subDict("subModels").subDict("injectionModels").subDict("model1").lookup("direction")),
    //OH_max(readScalar(dict.lookup("OH_max"))),
    criterion(dict.lookupOrDefault("criterion", 0.14)),
    ResultOutPut("LOL")
{
    ResultOutPut << "time," << "LOL (mm)" << endl;
    //Info<<"OH_max = "<<OH_max<<endl;
    Info<<"criterion = "<<criterion<<endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::LOL::~LOL()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// useless:
bool Foam::functionObjects::LOL::clear(){return true;}


bool Foam::functionObjects::LOL::execute()
{
    if (foundObject<volScalarField>(fieldName_))
    {
        const volScalarField& OH = lookupObject<volScalarField>(fieldName_);
        const scalar OH_max = max(OH).value();

        scalar LOL = great;
        forAll (OH, cellI)
        {
            if (OH[cellI] >= criterion*OH_max)
            {
                vector raw = position - mesh_.C()[cellI];

                if (mag(raw&direction) < LOL)
                {
                    LOL = mag(raw&direction);
                }
            }
        }
        reduce(LOL, minOp<scalar>());
        Info << "LOL = " << LOL*1000 << " mm" << endl;
        ResultOutPut << mesh_.time().value() << "," << LOL*1000 << endl;
    }
    else
    {
        Info << "Sorry! I cannot found OH!" << endl;
    }

    return true;
}

bool Foam::functionObjects::LOL::write()
{
    return true;
}


// ************************************************************************* //