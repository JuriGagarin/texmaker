/***************************************************************************
 *   copyright       : (C) 2003-2011 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *   contains some code from CLedit (C) 2010 Heinz van Saanen -GPL         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "blockdata.h"


BlockData::~BlockData()
{
    qDeleteAll(m_latexblocks);
    qDeleteAll(m_parentheses);
}

const QVector<ParenthesisInfo *> &BlockData::parentheses() {
	return m_parentheses;
}
const QVector<LatexBlockInfo *> &BlockData::latexblocks() {
	return m_latexblocks;
}

void BlockData::insertPar( ParenthesisInfo *info ) {
    insertType(m_parentheses, info);
}

void BlockData::insertLat( LatexBlockInfo *info ) {
    insertType(m_latexblocks, info);
}

template <typename T>
void BlockData::insertType(QVector<T*>& vec, T *info)
{
    int i = 0;
    while (i < vec.size() &&
        info->position > vec.at(i)->position )
        ++i;
    //TODO there may be a memory leak if vec[i] returns a valid pointer which will not get deleted
    //I tried to do some checks but it had some side effects
    vec.insert( i, info );
}
