/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "engineTimeActivatedFileUpdate.H"
#include "Time.H"
#include "polyMesh.H"
#include "addToRunTimeSelectionTable.H"
#include "IFstream.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(engineTimeActivatedFileUpdate, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        engineTimeActivatedFileUpdate,
        dictionary
    );
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::functionObjects::engineTimeActivatedFileUpdate::updateFile()
{
    label i = lastIndex_;

    while
    (
        i < timeVsFile_.size()-1
     && timeVsFile_[i+1].first() < timeToDeg(time_.value())
    )
    {
        i++;
    }

    if (i > lastIndex_)
    {
        Info<< nl << type() << ": copying file" << nl << timeVsFile_[i].second()
            << nl << "to:" << nl << fileToUpdate_ << nl << endl;

        fileName destFile(fileToUpdate_ + Foam::name(pid()));
        cp(timeVsFile_[i].second(), destFile);
        mv(destFile, fileToUpdate_);
        lastIndex_ = i;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::engineTimeActivatedFileUpdate::engineTimeActivatedFileUpdate
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObject(name),
    time_(runTime),
    rpm_(dictionary(IFstream(runTime.constant()+"/engineGeometry")()).lookup("rpm")),
    fileToUpdate_(dict.lookup("fileToUpdate")),
    timeVsFile_(),
    lastIndex_(-1)
{
    read(dict);

    Info << "rpm in engineTimeActivatedFileUpdate ===" << rpm_.value() << endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::engineTimeActivatedFileUpdate::~engineTimeActivatedFileUpdate()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::engineTimeActivatedFileUpdate::read
(
    const dictionary& dict
)
{
    dict.lookup("fileToUpdate") >> fileToUpdate_;
    dict.lookup("timeVsFile") >> timeVsFile_;

    lastIndex_ = -1;
    fileToUpdate_.expand();

    Info<< type() << ": time vs file list:" << nl;
    forAll(timeVsFile_, i)
    {
        timeVsFile_[i].second() = timeVsFile_[i].second().expand();
        if (!isFile(timeVsFile_[i].second()))
        {
            FatalErrorInFunction
                << "File: " << timeVsFile_[i].second() << " not found"
                << nl << exit(FatalError);
        }

        Info<< "    " << timeVsFile_[i].first() << tab
            << timeVsFile_[i].second() << endl;
    }
    Info<< endl;

    updateFile();

    return true;
}


bool Foam::functionObjects::engineTimeActivatedFileUpdate::execute()
{
    updateFile();

    return true;
}

Foam::scalar Foam::functionObjects::engineTimeActivatedFileUpdate::timeToDeg(const scalar t)
{
    return t*(6.0*rpm_.value());
}

bool Foam::functionObjects::engineTimeActivatedFileUpdate::write()
{
    return true;
}


// ************************************************************************* //
