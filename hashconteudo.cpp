#include "hashconteudo.h"


HashConteudo::HashConteudo(QString identificador, QString cat,int nvLex, int escopo, QString tipo, int deslocamento)
{
    this->parametrosFormais = NULL;
    this->procedureFunction = NULL;

    this->variavel = new StructHashVariavel;
    this->variavel->identificador = identificador;
    this->variavel->categoria = cat;
    this->variavel->nivelLexico = nvLex;
    this->variavel->escopo = escopo;
    this->variavel->tipo = tipo;
    this->variavel->deslocamento = deslocamento;
}

HashConteudo::HashConteudo( QString ident, QString cat,int nvLex,int escopo, QString tipo, int deslocamento, bool passagem)
{
    this->procedureFunction = NULL;
    this->variavel = NULL;

    this->parametrosFormais = new StructHashParametrosFormais;
    this->parametrosFormais->identificador = ident;
    this->parametrosFormais->categoria = cat;
    this->parametrosFormais->nivelLexico = nvLex;
    this->parametrosFormais->escopo = escopo;
    this->parametrosFormais->deslocamento = deslocamento;
    this->parametrosFormais->tipo = tipo;
    this->parametrosFormais->passagem = passagem;
}

HashConteudo::HashConteudo( QString ident, QString cat,int nvLex,int escopo, QString tipo, int deslocamento, QString ret,int qtdParam)
{
    this->parametrosFormais = NULL;
    this->variavel = NULL;

    this->procedureFunction = new StructHashProcedureFunction;
    this->procedureFunction->identificador = ident;
    this->procedureFunction->categoria = cat;
    this->procedureFunction->nivelLexico = nvLex;
    this->procedureFunction->idEscopo = escopo;
    this->procedureFunction->tipo = tipo;
    this->procedureFunction->deslocamento = deslocamento;
    this->procedureFunction->retorno = ret;
    this->procedureFunction->quantidadeParametros = qtdParam;
}

bool HashConteudo::compara(QString id, int nvLex, int escopo)const
{
    HashConteudo::tipoConteudo t;
    t = this->getConteudo();
    switch(t){
    case HashConteudo::VAR:
        return (this->variavel->identificador == id     &&
                this->variavel->nivelLexico   == nvLex  &&
                this->variavel->escopo        == escopo );
        break;
    case HashConteudo::PARAM:
        return (this->parametrosFormais->identificador == id     &&
                this->parametrosFormais->nivelLexico   == nvLex  &&
                this->parametrosFormais->escopo        == escopo );
        break;
    case HashConteudo::PROC_FUNC:
        return (this->procedureFunction->identificador == id     &&
                this->procedureFunction->nivelLexico   == nvLex  &&
                this->procedureFunction->idEscopo      == escopo );
        break;
    default:
        return false;
        break;
    }
}

HashConteudo::tipoConteudo HashConteudo::getConteudo()const
{
    if(this->variavel!=NULL)
    {
        return HashConteudo::VAR;
    }
    else if(this->parametrosFormais!=NULL)
    {
        return HashConteudo::PARAM;
    }
    else if(this->procedureFunction!=NULL)
    {
        return HashConteudo::PROC_FUNC;
    }
    else
    {
        return HashConteudo::NADA;
    }
}
QString HashConteudo::toString()
{
    QString retorno;
    HashConteudo::tipoConteudo t;
    t = this->getConteudo();
    switch(t){
    case HashConteudo::VAR:
        return varToStr();
        break;
    case HashConteudo::PARAM:
        return paramToStr();
        break;
    case HashConteudo::PROC_FUNC:
        return procToStr();
        break;
    default:
        break;
    }
    return retorno;
}
QString HashConteudo::varToStr()
{
    QString retorno;
    retorno = "Identificador: "+ variavel->identificador+"\n";
    retorno += "Categoria: "+ variavel->categoria+"\n";
    retorno += "Nível Lexico: "+ QString::number(variavel->nivelLexico)+"\n";
    retorno += "Tipo: "+ variavel->tipo+"\n";
    retorno += "Deslocamento: "+ QString::number(variavel->deslocamento)+"\n";
    retorno += "Escopo: "+ QString::number(variavel->escopo)+"\n";
    return retorno;

}

QString HashConteudo::paramToStr()
{
    QString retorno;
    retorno = "Identificador: "+ parametrosFormais->identificador+"\n";
    retorno += "Categoria: "+ parametrosFormais->categoria+"\n";
    retorno += "Nível Lexico: "+ QString::number(parametrosFormais->nivelLexico)+"\n";
    retorno += "Tipo: "+ parametrosFormais->tipo+"\n";
    retorno += "Deslocamento: "+ QString::number(parametrosFormais->deslocamento)+"\n";
    retorno += "Passagem: "+ QString::number(parametrosFormais->passagem)+"\n";
    retorno += "Passagem: "+ QString::number(parametrosFormais->passagem)+"\n";
    retorno += "Escopo: "+ QString::number(parametrosFormais->escopo)+"\n";
    return retorno;
}

QString HashConteudo::procToStr()
{
    QString retorno;
    retorno = "Identificador: "+ procedureFunction->identificador+"\n";
    retorno += "Categoria: "+ procedureFunction->categoria+"\n";
    retorno += "Nivel Lexico: "+ QString::number(procedureFunction->nivelLexico)+"\n";
    retorno += "Escopo: "+ QString::number(procedureFunction->idEscopo)+"\n";
    retorno += "Tipo: "+ procedureFunction->tipo+"\n";
    retorno += "Deslocamento: "+ QString::number(procedureFunction->deslocamento)+"\n";
    retorno += "Retorno: "+ procedureFunction->retorno+"\n";
    retorno += "Quantidade de Parametros: "+ QString::number(procedureFunction->quantidadeParametros)+"\n";

    return retorno;
}

