#ifndef NOTYPE_Meu
#define NOTYPE_Meu

#include<QString>
#include<QList>
#include<QDebug>
#include<QTreeWidgetItem>
class NoType
{
public:
    NoType(QString descr="", int nvLex=0, bool term = false);
    ~NoType();
    QString getDescricao();
    void setDescricao(QString desc);

    int getNivelLexico();
    void setNivelLexico(int nvLexico);

    bool getIsTerminal();
    void setIsTerminal(bool terminal);

    void insereFilho(NoType * filho);
    void insereFilho(QString descr="", int nvLex=0, bool term = false);
    void print();
    void toTree(QTreeWidgetItem * item);
private:
    QString descricao;
    int nivelLexico;
    bool isTerminal;
    QList<NoType*> filhos;
};
#endif // NOTYPE_H
