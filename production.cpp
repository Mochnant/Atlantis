// START A3HEADER
//
// This source file is part of the Atlantis PBM game program.
// Copyright (C) 1995-1999 Geoff Dunbar
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program, in the file license.txt. If not, write
// to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// See the Atlantis Project web page for details:
// http://www.prankster.com/project
//
// END A3HEADER
#include "production.h"
#include "fileio.h"
#include "gamedefs.h"
#include "gameio.h"
#include "items.h"
#include "gamedata.h"
#include "astring.h"

Production::Production()
{
	itemtype = -1;
	amount = 0;
	baseamount = 0;
	level = 0;
	productivity = 0;
	skill = -1;
}

Production::Production(int it, int maxamt)
{
	itemtype = it;
	amount = maxamt;
	if (Globals->RANDOM_ECONOMY)
	{
		amount += getrandom(maxamt);
	}
	baseamount = amount;
	level = 1;
	productivity = 1;
	skill = ItemDefs[it].pSkill;
}

void Production::Writeout(Aoutfile *f)
{
	f->PutInt(itemtype);
	f->PutInt(amount);
	f->PutInt(baseamount);
	f->PutInt(skill);
	f->PutInt(level);
	f->PutInt(productivity);
}

void Production::Readin(Ainfile *f)
{
	itemtype = f->GetInt();
	amount = f->GetInt();
	baseamount = f->GetInt();
	skill = f->GetInt();
	level = f->GetInt();
	productivity = f->GetInt();
}

AString Production::WriteReport()
{
	return ItemString(itemtype, amount);
}

//----------------------------------------------------------------------------
void ProductionList::Writeout(Aoutfile *f)
{
	f->PutInt(Num());
	forlist(this)
	{
		((Production*)elem)->Writeout(f);
	}
}

void ProductionList::Readin(Ainfile *f)
{
	const int n = f->GetInt();
	for (int i = 0; i < n; ++i)
	{
		Production *p = new Production;
		p->Readin(f);
		Add(p);
	}
}

Production* ProductionList::GetProd(int t, int s)
{
	forlist(this)
	{
		Production *p = (Production*)elem;
		if (p->itemtype == t && p->skill == s)
			return p;
	}
	return NULL;
}

void ProductionList::AddProd(Production *p)
{
	Production *p2 = GetProd(p->itemtype, p->skill);
	if (p2)
	{
		Remove(p2);
		delete p2;
	}

	Add(p);
}

