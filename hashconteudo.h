#ifndef HASHCONTEUDO_H
#define HASHCONTEUDO_H
#include<QString>


typedef struct StructHashVariavel
{
    QString identificador;
    QString categoria;
    int nivelLexico;
    QString tipo;
    int deslocamento;
    int escopo;
};

typedef struct StructHashParametrosFormais
{
    QString identificador;
    QString categoria;
    int nivelLexico;
    QString tipo;
    int deslocamento;
    bool passagem;
    int escopo;
};

typedef struct StructHashProcedureFunction
{
        QString identificador;
        QString categoria;
        int nivelLexico;
        int idEscopo;
        QString tipo;
        int deslocamento;
        QString retorno;
        int quantidadeParametros;
};

class HashConteudo
{
public:
    enum tipoConteudo{VAR, PARAM, PROC_FUNC, NADA};
    HashConteudo( QString identificador, QString cat, int nvLex, int escopo, QString tipo, int deslocamento);
    HashConteudo( QString ident, QString cat,int nvLex,int escopo, QString tipo, int deslocamento, bool passagem);
    HashConteudo( QString ident, QString cat,int nvLex,int idEscopo, QString tipo, int deslocamento, QString ret,int qtdParam);

    tipoConteudo getConteudo() const;
    StructHashVariavel* variavel;
    StructHashProcedureFunction* procedureFunction;
    StructHashParametrosFormais* parametrosFormais;
    QString toString();
    bool compara(QString id, int nvLex, int escopo) const;
private:
    QString varToStr();
    QString paramToStr();
    QString procToStr();
};

#endif // HASHCONTEUDO_H
