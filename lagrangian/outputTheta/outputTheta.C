/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
July 2019: Bruno Santos @ FSD blueCAPE Lda
           - created function object based on OpenFOAM's cloudInfo
November 2019: Bruno Santos @ FSD blueCAPE Lda
           - added support for MPPIC clouds
Apirl 2020: Yan Zhang @ Lund University
           - added support for spray clouds
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

\*---------------------------------------------------------------------------*/

#include "outputTheta.H"

#include "basicSprayCloud.H"
#include "basicReactingParcel.H"

#include "dictionary.H"
#include "PstreamReduceOps.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(outputTheta, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        outputTheta,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::outputTheta::outputTheta
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    regionFunctionObject(name, runTime, dict)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::outputTheta::~outputTheta()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::outputTheta::read(const dictionary& dict)
{
    regionFunctionObject::read(dict);

    names = wordList(dict.lookup("clouds"));

    Info<< type() << " " << name() << ": ";
    if (names.size())
    {
        Info<< "applying to clouds:" << nl;
        forAll(names, i)
        {
            Info<< "    " << names[i] << nl;
        }
        Info<< endl;
    }
    else
    {
        Info<< "no clouds to be processed" << nl << endl;
    }

    return true;
}


bool Foam::functionObjects::outputTheta::execute()
{
    return true;
}


bool Foam::functionObjects::outputTheta::write()
{
    forAll(names, i)
    {
        const word& cloudName = names[i];

        const kinematicCloud& kcloud =
            obr_.lookupObject<kinematicCloud>(cloudName);


        if (isA<basicSprayCloud>(kcloud))
        {
            const basicSprayCloud& cloud =
                dynamic_cast<const basicSprayCloud&>(kcloud);

            cloud.theta()().write();
        }      

    }

    return true;
}


// ************************************************************************* //
