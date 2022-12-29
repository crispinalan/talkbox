/***************************************************************************
 GPL v3.0 license (see license document distributed with this software)
 Author: crispinalan@gmail.com
 ***************************************************************************/


#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>
#include <QUrl>
#include <QDebug>
#include <QHash>
#include <QApplication>
#include <QDate>
#include <QList>


class Dictionary
{
public:
	Dictionary();
	
	QString wordToDiphones(QString);

private: 

QHash<QString, QString> diphoneDictionary;

void setupDictionary();


};   

#endif //DICTIONARY_H
