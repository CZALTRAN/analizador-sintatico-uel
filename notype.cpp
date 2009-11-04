#include "notype.h"

NoType::NoType(QString descr, int nvLex, bool term)
{
    this->descricao = descr;
    this->nivelLexico = nvLex;
    this->isTerminal = term;
}
NoType::~NoType()
{
    for(int i=0;i<filhos.count();i++)
        delete filhos.takeAt(i);
}
//-----------------------------------------------------------------------------------------
QString NoType::getDescricao()
{
    return this->descricao;
}
//-----------------------------------------------------------------------------------------
void NoType::setDescricao(QString desc)
{
    this->descricao = desc;
}
//-----------------------------------------------------------------------------------------
int NoType::getNivelLexico()
{
    return this->nivelLexico;
}
//-----------------------------------------------------------------------------------------
void NoType::setNivelLexico(int nvLexico)
{
    this->nivelLexico = nvLexico;
}
//-----------------------------------------------------------------------------------------
bool NoType::getIsTerminal()
{
    return this->isTerminal;
}
//-----------------------------------------------------------------------------------------
void NoType::setIsTerminal(bool terminal)
{
    this->isTerminal = terminal;
}
//-----------------------------------------------------------------------------------------
void NoType::insereFilho(NoType * filho)
{
    if(filho != NULL)
        this->filhos.append(filho);
}
//-----------------------------------------------------------------------------------------
void NoType::insereFilho(QString descr, int nvLex, bool term)
{
    this->insereFilho(new NoType(descr,nvLex, term));
}
//-----------------------------------------------------------------------------------------
void NoType::print()
{
    qDebug()<<this->getDescricao();
    for(int i=0;i<this->filhos.count();i++){
        if(filhos[i]!=NULL)
            filhos[i]->print();
    }
}
//-----------------------------------------------------------------------------------------
void NoType::toTree(QTreeWidgetItem *item)
{
    item->setText(0,this->descricao);
    item->setText(1,QString::number(this->nivelLexico));
    item->setText(2,(this->isTerminal ? "sim":"nao"));
    for(int i=0;i<this->filhos.count();i++){
        QTreeWidgetItem * temp = new QTreeWidgetItem();
        if(filhos[i]!=NULL){
            filhos[i]->toTree(temp);
            item->addChild(temp);
        }
    }

}
//-----------------------------------------------------------------------------------------
