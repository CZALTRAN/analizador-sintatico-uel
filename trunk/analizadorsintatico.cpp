#include "analizadorsintatico.h"

AnalizadorSintatico::AnalizadorSintatico()
{
}
void AnalizadorSintatico::analizar(QList<Tokens*>* lex)
{
    this->tokens = lex;
    this->nvLexico = 0;
    this->tokensPointer = 0;
    this->arvore->setRaiz(cs_programa());

    this->arvore->print();
}
NoType * AnalizadorSintatico::cs_programa()
{
    NoType * retorno = new NoType("PROGRAMA", nvLexico, false);
    if(tokens->at(tokensPointer)->token=="PA_PROGRAM")
    {
        retorno->insereFilho(new NoType(tokens->at(tokensPointer)->token,nvLexico,true));
        tokensPointer++;
        //Identificador
        if(tokens->at(tokensPointer)->Classificacao=="TO_ID")
        {
            retorno->insereFilho(cs_identificador());
            //';'
            if(tokens->at(tokensPointer)->token=="OP_PTVG")
            {
                retorno->insereFilho(new NoType(tokens->at(tokensPointer)->token,nvLexico,true));
                tokensPointer++;
                //Bloco
                retorno->insereFilho(cs_bloco());
                if(currentToken()->token=="OP_PTO"){
                    retorno->insereFilho(new NoType(currentToken()->token,nvLexico,true));
                }
                else
                {
                    this->erro("esperado '.' ao final do arquivo");
                }
            }
            else
            {
                this->erro(tokens->at(tokensPointer)->linha,"';' esperado apos identificador");
            }
        }
        else
        {
            this->erro(tokens->at(tokensPointer)->linha,"identificador esperado apos 'program'");
        }
    }
    else
    {
        this->erro(tokens->at(tokensPointer)->linha,"palavra 'program' esperada");
    }
    return retorno;
}

NoType * AnalizadorSintatico::cs_identificador()
{
    NoType * retorno = new NoType("IDENTIFICADOR",nvLexico,false);
    retorno->insereFilho(new NoType(currentToken()->token,nvLexico,true));
    this->tokensPointer++;
    return retorno;
}
NoType * AnalizadorSintatico::cs_bloco()
{
    NoType * retorno = new NoType("BLOCO",nvLexico,false);

    //retorno->insereFilho( this->parteDeclaracoesVariaveis() );
    //retorno->insereFilho( this->parteDeclaracoesSubRotinas() );

    if( this->currentToken()->token == "PA_BEGIN" )
    {
        //retorno->insereFilho( this->comandoComposto() );
    }
    else
    {
        erro("Esperado: 'begin' apos ';'" );
    }

    return retorno;
}

void AnalizadorSintatico::erro(int linha, QString msg)
{
        this->errors.append("Linha "+QString::number(linha)+" - "+msg);
}
void AnalizadorSintatico::erro(QString msg)
{
    this->errors.append("Linha "+QString::number(currentToken()->linha)+" - "+msg);
}
Tokens * AnalizadorSintatico::currentToken()
{
    return tokens->at(tokensPointer);
}
ArvoreSintatica * AnalizadorSintatico::getArvore()
{
    return arvore;
}
