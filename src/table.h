#ifndef MARKTABLE_H
#define MARKTABLE_H
#include <QString>
#include <QMetaType>

typedef struct tableIdentity {
    long locate;
	QString name;
	QString type;
	QString address;
	struct tableIdentity *next;
}Identifier;

typedef struct tableConstant {
    long locate;
	QString name;
	QString value;
	struct tableConstant *next;
}Constant;

class MarkTable {
private :
    Identifier *headId;
    Constant *headConst;
    long countId;
    long countConst;
public :
    MarkTable();
	long appendId(QString &id);//普通的节点插入法
	long appendConst(QString &constant);
	void checkId();//从前往后扫描
	void checkConst();
	//void removeIdAll();
	//void removeConstAll();
	void displayIdAll();//in table //
	void displayConstAll();
	void displayIdItem();
	void displayConstItem();
};
#endif
