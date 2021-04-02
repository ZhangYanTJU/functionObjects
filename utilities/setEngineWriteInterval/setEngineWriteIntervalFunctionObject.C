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
\*---------------------------------------------------------------------------*/

#include "setEngineWriteIntervalFunctionObject.H"
#include "addToRunTimeSelectionTable.H"
#include "IFstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(setEngineWriteIntervalFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        setEngineWriteIntervalFunctionObject,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::setEngineWriteIntervalFunctionObject::
setEngineWriteIntervalFunctionObject
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObject(name),
    time_(runTime),
    SOI_(readScalar(dictionary(IFstream(runTime.constant()+"/sprayCloudProperties")()).subDict("subModels").subDict("injectionModels").subDict("model1").lookup("SOI")))
{
    read(dict);
    Info<< "SOI_ = " << SOI_ << endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::setEngineWriteIntervalFunctionObject::
~setEngineWriteIntervalFunctionObject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::setEngineWriteIntervalFunctionObject::read
(
    const dictionary& dict
)
{
    return true;
}


bool Foam::functionObjects::setEngineWriteIntervalFunctionObject::execute()
{
    const scalar CA = time_.timeOutputValue();
    if (CA < SOI_ - 1) const_cast<Time&>(time_).setWriteInterval(time_.userTimeToTime(10.));
    if (CA > SOI_ - 1) const_cast<Time&>(time_).setWriteInterval(time_.userTimeToTime(1.));
    if (CA > SOI_ + 20) const_cast<Time&>(time_).setWriteInterval(time_.userTimeToTime(2.));
    if (CA > -10) const_cast<Time&>(time_).setWriteInterval(time_.userTimeToTime(0.25));
    if (CA > 20) const_cast<Time&>(time_).setWriteInterval(time_.userTimeToTime(10.));

    return true;
}


bool Foam::functionObjects::setEngineWriteIntervalFunctionObject::write()
{
    return true;
}


// ************************************************************************* //
