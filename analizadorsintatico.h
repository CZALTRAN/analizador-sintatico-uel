#ifndef ANALIZADORSINTATICO_H
#define ANALIZADORSINTATICO_H
#include "qmyhash.h"
#include "arvoresintatica.h"
#include "tokens.h"
#include <QHash>
#include <QStringList>
class AnalizadorSintatico
{
public:
    AnalizadorSintatico();
    void analizar(QList<Tokens*>*);
    ArvoreSintatica * getArvore();

private:
    int escopo;
    int nvLexico;
    int tokensPointer;
    QList<Tokens * > *tokens;
    ArvoreSintatica * arvore;
    QMyHash hash;
    QStringList errors;

    //Funções de analize (cartas Sintaticas)
    NoType * cs_programa();
    NoType * cs_identificador();
    NoType * cs_bloco();
    NoType * cs_tipo();
    NoType * cs_parametrosFormais(QList<HashConteudo *> &vars);
    NoType * cs_comando();
    NoType * cs_numero();
    NoType * cs_comandoSemRotulo();
    NoType * cs_expressao();
    NoType * cs_expressaoSimples();
    NoType * cs_termo();
    NoType * cs_fator();

    //Sub funções de analize
    NoType * sub_declaracoesVariaveis();
    NoType * sub_declaracaoVariaveis(QList<HashConteudo *> &vars);
    NoType * sub_listaIdentificadores(QList<HashConteudo *> &vars);
    NoType * sub_listaIdentificadores(bool ref, QList<HashConteudo *> &vars);
    NoType * sub_declaracoesSubRotinas();
    NoType * sub_declaracaoFuncao();
    NoType * sub_secaoParametrosFormais(QList<HashConteudo *> &vars);
    NoType * sub_declaracaoProcedimento();
    NoType * sub_comandoComposto();
    NoType * sub_chamadaProcedimento();
    NoType * sub_listaExpressoes();
    NoType * sub_variavel();
    NoType * sub_chamadaFuncao();
    NoType * sub_relacao();
    NoType * sub_atribuicao();
    NoType * sub_comandoEscrita();
    NoType * sub_comandoLeitura();
    NoType * sub_comandoCondicional();
    NoType * sub_comandoRepetitivo();

    //Funções auxiliares
    void aux_inserirParametrosFormaisHash(QList<HashConteudo*> &list);
    void aux_setaTipoLista(QList<HashConteudo*> &list, QString tipo);
    void aux_inserirVariaveisHash(QList<HashConteudo*> &list);
    bool oprRel(QString token);
    void erro(int linha, QString msg);
    void erro(QString msg);
    Tokens * currentToken();

};

#endif // ANALIZADORSINTATICO_H
