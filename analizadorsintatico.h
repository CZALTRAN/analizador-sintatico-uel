#ifndef ANALIZADORSINTATICO_H
#define ANALIZADORSINTATICO_H
#include "arvoresintatica.h"
#include "tokens.h"
#include <QStringList>
class AnalizadorSintatico
{
public:
    AnalizadorSintatico();
    void analizar(QList<Tokens*>*);
    ArvoreSintatica * getArvore();
private:
    int nvLexico;
    int tokensPointer;
    QList<Tokens * > *tokens;
    ArvoreSintatica * arvore;
    QStringList errors;

    //Funções de analize (cartas Sintaticas)
    NoType * cs_programa();
    NoType * cs_identificador();
    NoType * cs_bloco();

    //Sub funções de analize
    NoType * sub_declaracoesRotulos();
    //Funções auxiliares
    void erro(int linha, QString msg);
    void erro(QString msg);
    Tokens * currentToken();

};

#endif // ANALIZADORSINTATICO_H
