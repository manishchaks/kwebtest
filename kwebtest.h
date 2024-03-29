/***************************************************************************
		*   Copyright (C) 2007 by Manish Chakravarty   *
		*   manish@freedom   *
		*                                                                         *
		*   This program is free software; you can redistribute it and/or modify  *
		*   it under the terms of the GNU General Public License as published by  *
		*   the Free Software Foundation; either version 2 of the License, or     *
		*   (at your option) any later version.                                   *
		*                                                                         *
		*   This program is distributed in the hope that it will be useful,       *
		*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
		*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
		*   GNU General Public License for more details.                          *
		*                                                                         *
		*   You should have received a copy of the GNU General Public License     *
		*   along with this program; if not, write to the                         *
		*   Free Software Foundation, Inc.,                                       *
		*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/


#ifndef _KWEBTEST_H_
#define _KWEBTEST_H_

#include "kwebtestincludes.h"
#include <qlistview.h>

/**
\ingroup MainModule
\class kwebtest
\brief The Main window for KWebTest
*/
class kwebtest : public KMainWindow
{
Q_OBJECT
public:
	kwebtest();

	virtual ~kwebtest();
};

#endif // _KWEBTEST_H_
