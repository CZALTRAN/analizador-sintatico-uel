#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>
#include <QtCore/QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "tokens.h"
#include "analizadorsintatico.h"
void Transforma(QFile &arquivo, QList<Tokens*> *tokens);
void MostrarArvoreSintatica(ArvoreSintatica *ar);
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QFile f("../AnalizadorLexico/saida.out");
    f.open( QIODevice::ReadOnly);
    QList<Tokens*> l;
    Transforma(f,&l);
    AnalizadorSintatico as;
    as.analizar(&l);
    MostrarArvoreSintatica(as.getArvore());
    return a.exec();

}
void Transforma(QFile &arquivo, QList<Tokens*> * tokens)
{
    QTextStream tsFile(&arquivo);
    while(!tsFile.atEnd()){
        QString line = tsFile.readLine();
        //remove o nome do arquivo
        while(line[0]!=':'){
            line.remove(0,1);
        }
        line.remove(0,1);
        //tranforma string em Stream
        QTextStream tsLine(&line,QIODevice::ReadOnly);
        //Contador de linhas
        int lineCount;
        //le o numero da linha
        tsLine>>lineCount;
        //remove caracteres de controle
        line.remove(0,2);
        line.replace(',',' ');
        line.replace('[',' ');
        line.replace(']',' ');
        //lê os tokens
        while(!tsLine.atEnd()){
        //remove '['
            Tokens * temp = new Tokens();
            temp->linha = lineCount;
            tsLine>>temp->Classificacao;
            tsLine>>temp->token;
            tokens->append(temp);
        }

    }
}
void MostrarArvoreSintatica(ArvoreSintatica *ar)
{
    QStringList headers;
    headers.append("Descricao");
    headers.append("Nivel Lexico");
    headers.append("Terminal");
    QTreeWidget *tree = new QTreeWidget(0);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    ar->toTree(item);
    tree->setColumnCount(3);
    tree->setHeaderLabels(headers);
    tree->addTopLevelItem(item);
    tree->show();
}
