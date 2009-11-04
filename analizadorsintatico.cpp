#include "analizadorsintatico.h"

AnalizadorSintatico::AnalizadorSintatico()
{
}
void AnalizadorSintatico::analizar(QList<Tokens*>* lex)
{
    this->tokens = lex;
    this->nvLexico = 0;
    this->escopo = 0;
    this->tokensPointer = 0;
    this->arvore->setRaiz(cs_programa());

    this->arvore->print();
    for(int i=0;i<errors.size();i++){
        qDebug()<<errors[i];
    }
    this->hash.print();

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
    NoType * retorno = new NoType("<identificador>",nvLexico,false);
    retorno->insereFilho(new NoType(currentToken()->token,nvLexico,true));
    this->tokensPointer++;
    return retorno;
}
NoType * AnalizadorSintatico::cs_bloco()
{
    NoType * retorno = new NoType("<bloco>",nvLexico,false);

    retorno->insereFilho( this->sub_declaracoesVariaveis() );
    retorno->insereFilho( this->sub_declaracoesSubRotinas() );

    if( this->currentToken()->token == "PA_BEGIN" )
    {
        retorno->insereFilho( this->sub_comandoComposto() );
    }
    else
    {
        erro("Esperado: 'begin' apos ';'" );
    }

    return retorno;
}

NoType* AnalizadorSintatico::sub_declaracoesVariaveis()
{
    NoType * retorno = new NoType("<parte_declaracoes_variavel>",nvLexico,0);
    QList<HashConteudo *> temp;

    if( this->currentToken()->token== "PA_VAR" )
    {
        retorno->insereFilho(new NoType(this->currentToken()->token, this->nvLexico, true ));
        this->tokensPointer++;

        if( this->currentToken()->Classificacao != "TO_ID" )
        {
            erro("Esperado: identificador apos 'var'");
        }
        while(  this->currentToken()->Classificacao == "TO_ID")
        {
            retorno->insereFilho( this->sub_declaracaoVariaveis(temp) );

            if( this->currentToken()->token == "OP_PTVG" )
            {
                retorno->insereFilho( new NoType(this->currentToken()->token, this->nvLexico, true ));
                this->tokensPointer++;


                this->aux_inserirVariaveisHash(temp);

            }
            else
            {
                erro("Esperado ';'");
            }
        }
    }
    else
    {
        return NULL;
    }

    return retorno;
}

NoType * AnalizadorSintatico::sub_declaracaoVariaveis(QList<HashConteudo *> &vars)
{
    NoType* retorno = new NoType( "<declaracao variaveis>", this->nvLexico, false );
    QString aux_erro;
    if( this->currentToken()->Classificacao == "TO_ID" )
    {
        aux_erro = currentToken()->token;
        retorno->insereFilho( this->sub_listaIdentificadores(vars) );

        if( this->currentToken()->token == "OP_2PTO" )
        {
            retorno->insereFilho( new NoType(this->currentToken()->token, this->nvLexico,true ));
            this->tokensPointer++;

            if( currentToken()->Classificacao == "TO_ID")
            {
                this->aux_setaTipoLista(vars,currentToken()->token);

                retorno->insereFilho( this->cs_tipo() );
            }
            else
            {
                erro("Esperado tipo de variavel");
            }
        }
        else
        {
            this->tokensPointer--;
            erro("Esperado ':' apos '" + aux_erro + "'" );
            this->tokensPointer++;
        }
    }
    else
    {
        erro("Esperado identificador apos 'var'" );
    }

    return retorno;
}
void AnalizadorSintatico::aux_setaTipoLista(QList<HashConteudo*> &list, QString tipo){
    HashConteudo * temp;
    foreach(temp,list){
        if(temp->getConteudo()==HashConteudo::VAR)
        {
            temp->variavel->tipo =tipo;
        }
        else if(temp->getConteudo()==HashConteudo::PROC_FUNC)
        {
            temp->procedureFunction->tipo = tipo;
        }
        else if(temp->getConteudo()==HashConteudo::PARAM)
        {
            temp->parametrosFormais->tipo = tipo;
        }
    }
}
void AnalizadorSintatico::aux_inserirVariaveisHash(QList<HashConteudo*> &list)
{
    int desloc = 0;
    HashConteudo * temp = 0;
    foreach(temp, list){
        if(!hash.exists(temp->variavel->identificador,temp->variavel->escopo,temp->variavel->nivelLexico))
        {
            temp->variavel->deslocamento = desloc;
            hash.insert(temp->variavel->identificador,temp);
        }
        else
        {
            erro("Declaração dupla");
        }
        desloc ++;
    }
    list.clear();
}

NoType * AnalizadorSintatico::cs_tipo()
{
    NoType * retorno = new NoType("<tipo>",nvLexico,false);
    retorno->insereFilho(this->cs_identificador());
    return retorno;
}
NoType * AnalizadorSintatico::sub_listaIdentificadores(QList<HashConteudo*>  &vars)
{
    NoType* retorno = new NoType( "<lista_identificadores>", this->nvLexico, false );

    HashConteudo * insert;

    if( currentToken()->Classificacao == "TO_ID" )
    {
        insert = new HashConteudo( this->currentToken()->token,"VARIAVEL_SIMPLES",this->nvLexico,this->escopo,"",0 );

        vars.append(insert);

        //this->iteradorListaVariaveis = this->listaVariaveis.end();
        //--this->iteradorListaVariaveis;

        retorno->insereFilho( this->cs_identificador() );

        while( currentToken()->token == "OP_VIRG" )
        {
            retorno->insereFilho(new NoType( currentToken()->token, this->nvLexico, true) );
            this->tokensPointer++;

            if( currentToken()->Classificacao == "TO_ID" )
            {
                insert = new HashConteudo( currentToken()->token,"VARIAVEL_SIMPLES",this->nvLexico,this->escopo,"",0);

                vars.append(insert);

                retorno->insereFilho( this->cs_identificador() );
            }
            else
            {
                erro("Esperado identificador apos ','");
            }
        }
    }
    else
    {
        erro("Esperado identificador apos 'var'" );
    }
    return retorno;
}

NoType * AnalizadorSintatico::sub_declaracoesSubRotinas()
{
    NoType* retorno = new NoType( "<parte_declaracao_sub_rotinas>", this->nvLexico, false );

    if( (currentToken()->token != "PA_PROCEDURE") &&
        (currentToken()->token != "PA_FUNCTION" ) )
    {
        return NULL;
    }

    while( (currentToken()->token == "PA_PROCEDURE") ||
           (currentToken()->token == "PA_FUNCTION") )
    {
        if( currentToken()->token == "PA_PROCEDURE" )
        {
            retorno->insereFilho( this->sub_declaracaoProcedimento() );
        }
        else if( currentToken()->token == "PA_FUNCTION" )
        {
            retorno->insereFilho( this->sub_declaracaoFuncao() );
        }
    }
    return retorno;
}

NoType * AnalizadorSintatico::sub_declaracaoFuncao()
{
    NoType* retorno = new NoType( "<declaracao_funcao>", this->nvLexico, false );
    HashConteudo * insert;
    QList<HashConteudo * > temp;
    if( currentToken()->token == "PA_FUNCTION" )
    {
        retorno->insereFilho(new NoType( currentToken()->token, this->nvLexico, true ));
        tokensPointer++;
        this->nvLexico++;
        this->escopo++;

        if( currentToken()->Classificacao == "TO_ID" )
        {
            insert = new HashConteudo( currentToken()->token,"FUNCAO",this->nvLexico,this->escopo,"",0,"",0 );
            temp.append(insert);
            //this->listaVariaveis.push_back( *insert );
            //_posicaoIdentificadorFuncao = this->listaVariaveis.end();
            //--_posicaoIdentificadorFuncao;

            retorno->insereFilho( this->cs_identificador() );

            if( currentToken()->token == "OP_ABR_PAR" )
            {
                retorno->insereFilho( this->cs_parametrosFormais(temp) );
            }

            if( currentToken()->token == "OP_2PTO" )
            {
                retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                tokensPointer++;

                if( currentToken()->Classificacao == "TO_ID" )
                {
                    //_posicaoIdentificadorFuncao->procedureFunction->retorno = currentToken()->token;

                    retorno->insereFilho( this->cs_identificador() );

                    if( currentToken()->token == "OP_PTVG" )
                    {
                        retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                        tokensPointer++;

                        /*
                                                 * Insere na Hash Aqui!!!!!!!!!
                                                 */
                        this->aux_inserirParametrosFormaisHash(temp);

                        retorno->insereFilho( this->cs_bloco() );

                        --this->nvLexico;
                    }
                    else
                    {
                        erro("Esperado ';'" );
                        tokensPointer++;
                    }
                }
                else
                {
                    erro( "Esperado identificador apos ':'" );
                }
            }
            else
            {
                erro("Esperado ':' ");
                tokensPointer++;
            }
        }
        else
        {
            erro("Esperado identificador apos 'function'" );
        }
    }
    else
    {
        return NULL;
    }

    return retorno;
}
void AnalizadorSintatico::aux_inserirParametrosFormaisHash(QList<HashConteudo*> &list)
{
    int desloc = -3;

    unsigned int
    contParam = 0;
    HashConteudo * procPointer = 0;
    HashConteudo * temp = 0;
    for(int i = list.count() -1 ;i>=0;i--)
    {
        temp = list[i];
        if(temp->getConteudo()==HashConteudo::PARAM){
            if(!hash.exists(temp->parametrosFormais->identificador,temp->parametrosFormais->escopo,temp->parametrosFormais->nivelLexico))
            {
                temp->parametrosFormais->deslocamento = desloc;
                this->hash.insert(temp->parametrosFormais->identificador, temp);
            }else{
                erro("Declaração dupla");
            }
            contParam++;
        }
        else if(temp->getConteudo()==HashConteudo::PROC_FUNC)
        {
            if(!hash.exists(temp->procedureFunction->identificador,temp->procedureFunction->idEscopo,temp->procedureFunction->nivelLexico))
            {
                temp->procedureFunction->deslocamento = desloc;
                this->hash.insert(temp->procedureFunction->identificador, temp);
            }else{
                erro("Declaração dupla");
            }
            procPointer = temp;
        }
        desloc--;
    }
    procPointer->procedureFunction->quantidadeParametros= contParam;




    list.clear( );

}

NoType * AnalizadorSintatico::cs_parametrosFormais(QList<HashConteudo*> &vars)
{
    NoType* retorno = new NoType( "<paramentros_formais>", this->nvLexico, false );

    if( currentToken()->token == "OP_ABR_PAR" )
    {
        retorno->insereFilho( currentToken()->token, this->nvLexico, true );
        this->tokensPointer++;
        if( (currentToken()->Classificacao == "TO_ID") ||
            (currentToken()->token == "PA_VAR")        ||
            (currentToken()->token == "PA_FUNCTION")   ||
            (currentToken()->token == "PA_PROCEDURE")   )
        {
            retorno->insereFilho( this->sub_secaoParametrosFormais(vars) );
        }
        else
        {
            erro("Esperado parametro apos '('" );
        }
        while( currentToken()->token == "OP_PTVG" )
        {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            this->tokensPointer++;

            if(  (currentToken()->Classificacao == "TO_ID") ||
                 (currentToken()->token == "PA_VAR")        ||
                 (currentToken()->token == "PA_FUNCTION")   ||
                 (currentToken()->token == "PA_PROCEDURE")   )
            {
                retorno->insereFilho( this->sub_secaoParametrosFormais(vars) );
            }
            else
            {
                erro("Esperado parametro apos ';'" );
            }
        }
        if( currentToken()->token == "OP_FCH_PAR" )
        {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            this->tokensPointer++;
        }
        else
        {
            erro( "Esperado ')'");
        }
    }
    return retorno;
}
NoType * AnalizadorSintatico::sub_secaoParametrosFormais(QList<HashConteudo*> &vars)
{
    NoType * retorno = new NoType( "<secao_paramentros_formais>", this->nvLexico, false );

    if( currentToken()->Classificacao == "TO_ID" )
    {
        retorno->insereFilho( this->sub_listaIdentificadores( false,vars ));

        if( currentToken()->token == "OP_2PTO" )
        {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            if( currentToken()->Classificacao == "TO_ID" )
            {
                this->aux_setaTipoLista(vars, currentToken()->token );
                retorno->insereFilho( this->cs_identificador() );
            }
            else
            {
                erro(  "Esperado identificador apos ':'" );
            }
        }
        else
        {
            erro(  "Esperado: ':' " );
        }
    }
    else if( currentToken()->token == "PA_VAR" )
    {
        retorno->insereFilho( currentToken()->token, this->nvLexico, true );
        tokensPointer++;

        if( currentToken()->Classificacao == "TO_ID" )
        {
            retorno->insereFilho( this->sub_listaIdentificadores( true ,vars) );

            if( currentToken()->token == "OP_2PTO" )
            {
                retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                tokensPointer++;

                if( currentToken()->Classificacao == "TO_ID" )
                {
                    retorno->insereFilho( this->cs_identificador() );
                }
                else
                {
                    erro(  "Esperado identificador apos ':'" );
                }
            }
            else
            {
                erro(  "Esperado: ':' ");
            }
        }
        else
        {
            erro(  "Esperado identificador apos 'var'" );
        }
    }
    else if( currentToken()->token == "PA_FUNCTION" )
    {
        retorno->insereFilho( currentToken()->token, this->nvLexico, true );
        tokensPointer++;

        if( currentToken()->Classificacao == "TO_ID" )
        {
            retorno->insereFilho( this->sub_listaIdentificadores(vars) );

            if( currentToken()->token == "OP_2PTO" )
            {
                retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                tokensPointer++;

                if( currentToken()->Classificacao == "TO_ID" )
                {
                    retorno->insereFilho( this->cs_identificador() );
                }
                else
                {
                    erro(  "Esperado identificador apos ':'" );
                }
            }
            else
            {
                erro(  "Esperado: ':' ");
            }
        }
        else
        {
            erro(  "Esperado identificador apos 'function'" );
        }
    }
    else if( currentToken()->token == "PA_PROCEDURE" )
    {
        retorno->insereFilho( currentToken()->token, this->nvLexico, true );
        tokensPointer++;

        if( currentToken()->Classificacao == "TO_ID" )
        {
            retorno->insereFilho( this->sub_listaIdentificadores(vars) );
        }
        else
        {
            erro(  "Esperado identificador apos 'procedure'" );
        }
    }

    return retorno;
}

NoType * AnalizadorSintatico::sub_listaIdentificadores(bool ref, QList<HashConteudo *> &vars)
{
    NoType* retorno = new NoType( "<lista_identificadores>", this->nvLexico, false );
    HashConteudo* insert;

    if( currentToken()->Classificacao == "TO_ID" )
    {
        insert = new HashConteudo( currentToken()->token,"PARAMETRO_FORMAL",this->nvLexico,this->escopo,"",0,ref );
        vars.append(insert);
        //          this->listaVariaveis.push_back( *insert );
        //            this->iteradorListaVariaveis = this->listaVariaveis.end();
        //            --this->iteradorListaVariaveis;

        retorno->insereFilho( this->cs_identificador() );

        while( currentToken()->token == "OP_VIRG" )
        {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            if( currentToken()->Classificacao == "TO_ID" )
            {
                insert = new HashConteudo( currentToken()->token,"PARAMETRO_FORMAL",this->nvLexico,this->escopo,"",0,ref );

                vars.append(insert);

                retorno->insereFilho( this->cs_identificador() );
            }
            else
            {
                erro(  "Esperado identificador apos ','" );
            }
        }
    }
    else
    {
        erro(  "Esperado identificador apos 'var'" );
    }

    return retorno;

}
NoType * AnalizadorSintatico::sub_declaracaoProcedimento()
{
    NoType* retorno= new NoType( "<declaracao_procedimento>", this->nvLexico, false );

    HashConteudo* insert;
    QList<HashConteudo *> vars;
    if( currentToken()->token == "PA_PROCEDURE" )
    {
        retorno->insereFilho( currentToken()->token, this->nvLexico, true );
        tokensPointer++;
        this->nvLexico++;
        this->escopo++;


        if( currentToken()->Classificacao == "TO_ID" )
        {
            insert = new HashConteudo(currentToken()->token,"PROCEDIMENTO",this->nvLexico,this->escopo,"",0,"",0);
            vars.append(insert);
            //                    this->listaVariaveis.push_back( *insert );
            //                    retorno->insereFilho( this->identificador() );
            retorno->insereFilho(this->cs_identificador());
            if( currentToken()->token == "OP_ABR_PAR" )
            {
                retorno->insereFilho( this->cs_parametrosFormais(vars) );
            }
            qDebug()<<currentToken()->token;
            if( currentToken()->token == "OP_PTVG" )
            {
                retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                tokensPointer++;

                /*
                             * Insere na Hash Aqui!!!!!!!!!
                             */
                this->aux_inserirParametrosFormaisHash(vars);

                retorno->insereFilho( this->cs_bloco() );

                this->nvLexico--;
            }
            else
            {
                erro(  "Esperado ';'" );
            }
        }
        else
        {
            erro(  "Esperado identificador apos 'procedure'" );
        }
    }
    return retorno;
}
NoType * AnalizadorSintatico::sub_comandoComposto()
{
    NoType * retorno = new NoType( "<comando_composto>", this->nvLexico, false );

    if( currentToken()->token == "PA_BEGIN" )
    {
        retorno->insereFilho( currentToken()->token, this->nvLexico, true );
        tokensPointer++;

        retorno->insereFilho( this->cs_comando() );

        while( currentToken()->token == "OP_PTVG" )
        {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;
            retorno->insereFilho( this->cs_comando() );
        }

        if( currentToken()->token == "PA_END" )
        {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;
        }
        else
        {
            erro("Esperado 'end'" );
        }
    }
    else
    {
        erro("Esperado: 'begin'" );
    }
    return retorno;
}

NoType * AnalizadorSintatico::cs_comando()
{
    NoType* retorno = new NoType( "<comando>", this->nvLexico, false );

    if( currentToken()->Classificacao == "TO_IN" )
    {
        retorno->insereFilho( this->cs_numero() );
        if( currentToken()->token == "OP_2PTO" )
        {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            this->tokensPointer++;
        }
        else
        {
            erro("Esperado ':' apos numero" );
        }
    }
    retorno->insereFilho( this->cs_comandoSemRotulo() );
    return retorno;
}
NoType * AnalizadorSintatico::cs_comandoSemRotulo(){
    NoType* retorno = new NoType( "<comando_sem_rotulo>", this->nvLexico, false );
    if( currentToken()->token == "PA_WHILE" )
    {
        retorno->insereFilho( this->sub_comandoRepetitivo() );
    }
    else if( currentToken()->token == "PA_IF" )
    {
        retorno->insereFilho( this->sub_comandoCondicional() );
    }
    else if( currentToken()->token == "PA_BEGIN" )
    {
        retorno->insereFilho( this->sub_comandoComposto() );
    }
    else if( currentToken()->token == "PA_READ" )
    {
        retorno->insereFilho( this->sub_comandoLeitura() );
    }
    else if( currentToken()->token == "PA_WRITE" )
    {
        retorno->insereFilho( this->sub_comandoEscrita() );
    }
    else if( currentToken()->Classificacao == "TO_ID" )
    {
        if( this->tokens->at(tokensPointer+1)->token == "OP_ATRIB" )
        {
            retorno->insereFilho( this->sub_atribuicao() );
        }
        else
        {
            retorno->insereFilho( this->sub_chamadaProcedimento() );
        }
    }

    return retorno;
}

NoType * AnalizadorSintatico::cs_numero()
{
    NoType* retorno = new NoType( "<numero>", this->nvLexico, false );
    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
    this->tokensPointer++;

    return retorno;
}
NoType * AnalizadorSintatico::sub_chamadaProcedimento()
{
    NoType* retorno = new NoType( "<chamada_procedimento>", this->nvLexico, false );

    if( currentToken()->Classificacao == "TO_ID" )
    {
            retorno->insereFilho( this->cs_identificador() );

            if( currentToken()->token == "OP_ABR_PAR")
            {
                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                    tokensPointer++;

                    retorno->insereFilho( this->sub_listaExpressoes() );

                    if( currentToken()->token == "OP_FCH_PAR" )
                    {
                            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                            tokensPointer++;
                    }
                    else
                    {
                            erro( "Esperado ')'");
                    }
            }
    }
    else
    {
            erro("Esperado identificador");
    }

    return retorno;
}
NoType * AnalizadorSintatico::sub_listaExpressoes()
{
    NoType*
    retorno = new NoType( "<lista_expressoes>", this->nvLexico, false );

    retorno->insereFilho( this->cs_expressao() );

    while( currentToken()->token == "OP_VIRG" )
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            retorno->insereFilho( this->cs_expressao() );
    }
    return retorno;
}
NoType * AnalizadorSintatico::cs_expressao()
{
    NoType* retorno = new NoType( "<expressao>", this->nvLexico, false );

    retorno->insereFilho( this->cs_expressaoSimples() );

    if( oprRel(currentToken()->token))
    {
            retorno->insereFilho( this->sub_relacao() );
            retorno->insereFilho( this->cs_expressaoSimples() );
    }

    return retorno;
}
NoType * AnalizadorSintatico::cs_expressaoSimples()
{
    NoType* retorno = new NoType( "<expressao_simples>", this->nvLexico, false );

    if( (currentToken()->token == "OP_MAIS") ||
            (currentToken()->token == "OP_MENOS") )
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;
    }

    retorno->insereFilho( this->cs_termo() );

    while( (currentToken()->token == "OP_MAIS") ||
               (currentToken()->token == "OP_MENOS") ||
               (currentToken()->token == "PA_OR") )
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            retorno->insereFilho( this->cs_termo() );
    }

    return retorno;
}
NoType * AnalizadorSintatico::cs_termo()
{
    NoType* retorno = new NoType( "<termo>", this->nvLexico, false );

    retorno->insereFilho( this->cs_fator() );

    while( (currentToken()->token == "OP_VEZES")   ||
               (currentToken()->token == "OP_DIV") ||
               (currentToken()->token == "PA_AND") ||
               (currentToken()->token == "PA_DIV"))
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            retorno->insereFilho( this->cs_fator() );
    }

    return retorno;
}
NoType * AnalizadorSintatico::cs_fator()
{
    NoType* retorno = new NoType( "<fator>", this->nvLexico, false );

    if( currentToken()->token == "PA_NOT")
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            retorno->insereFilho( this->cs_fator() );
    }
    else if( currentToken()->token == "OP_ABR_PAR" )
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            retorno->insereFilho( this->cs_expressao() );

            if( currentToken()->token == "OP_FCH_PAR" )
            {
                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                    tokensPointer++;
            }
            else
            {
                    erro( "Esperado ')'" );
            }
    }
    else if( currentToken()->Classificacao == "TO_IN" )
    {
            retorno->insereFilho( this->cs_numero() );
    }
    else if( currentToken()->Classificacao == "TO_ID" )
    {
        if(tokens->at(tokensPointer+1)->token==("OP_ABR_PAR"))
        {
            retorno->insereFilho(this->sub_chamadaFuncao());
        }
        else
        {
            retorno->insereFilho(this->sub_variavel());
        }
    }
    return retorno;
}
NoType * AnalizadorSintatico::sub_variavel()
{
    NoType* retorno = new NoType( "<variavel>", this->nvLexico, false );

    retorno->insereFilho( this->cs_identificador() );

    return retorno;
}
NoType * AnalizadorSintatico::sub_chamadaFuncao()
{
    NoType* retorno = new NoType( "<chamada_funcao>", this->nvLexico, false );

    retorno->insereFilho( this->cs_identificador() );

    if( currentToken()->token == "OP_ABR_PAR" )
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            retorno->insereFilho( this->sub_listaExpressoes() );

            if( currentToken()->token == "OP_FCH_PAR" )
            {
                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                    tokensPointer++;
            }
            else
            {
                    erro("Esperado ')'" );
            }
    }

    return retorno;
}
NoType * AnalizadorSintatico::sub_relacao(){
    NoType* retorno = new NoType( "<relacao>", this->nvLexico, false );

    if(oprRel(currentToken()->token))
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;
    }

    return retorno;
}
NoType * AnalizadorSintatico::sub_atribuicao()
{
    NoType* retorno = new NoType( "<atribuicao>", this->nvLexico, false );

    if( currentToken()->Classificacao == "TO_ID" )
    {
            retorno->insereFilho( this->sub_variavel() );

            if( currentToken()->token == "OP_ATRIB" )
            {
                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                    tokensPointer++;

                    retorno->insereFilho( this->cs_expressao() );
            }
            else
            {
                   erro("Esperado ':='");
            }
    }
    else
    {
            erro("Esperado identificador" );
    }

    return retorno;
}
NoType * AnalizadorSintatico::sub_comandoEscrita()
{
    NoType* retorno = new NoType( "<comando_escrita>", this->nvLexico, false );

    if( currentToken()->token == "PA_WRITE" )
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            if( currentToken()->token == "OP_ABR_PAR" )
            {
                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                    tokensPointer++;

                    retorno->insereFilho( this->cs_expressao() );

                    while( currentToken()->token == "OP_VIRG" )
                    {
                            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                            tokensPointer++;

                            retorno->insereFilho( this->cs_expressao() );
                    }

                    if( currentToken()->token == "OP_FCH_PAR" )
                    {
                            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                            tokensPointer++;
                    }
                    else
                    {
                            erro("Esperado ')'" );
                    }
            }
            else
            {
                    erro("Esperado '('" );
            }
    }

    return retorno;
}
NoType * AnalizadorSintatico::sub_comandoLeitura()
{
         NoType* retorno = new NoType( "<comando_leitura>", this->nvLexico, false );

            if( currentToken()->token == "PA_READ" )
            {
                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                    tokensPointer++;

                    if( currentToken()->token == "OP_ABR_PAR" )
                    {
                            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                            tokensPointer++;
                            QList<HashConteudo*> t;
                            retorno->insereFilho( this->sub_listaIdentificadores(t));

                            if( currentToken()->token == "OP_FCH_PAR" )
                            {
                                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                                    tokensPointer++;
                            }
                            else
                            {
                                    erro("Esperado ')'" );
                            }
                    }
                    else
                    {
                            erro("Esperado '('" );
                    }
            }

            return retorno;
}
NoType * AnalizadorSintatico::sub_comandoCondicional()
{
    NoType* retorno = new NoType( "<comando_condicional>", this->nvLexico, false );


            if( currentToken()->token == "PA_IF" )
            {
                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                    tokensPointer++;

                    retorno->insereFilho( this->cs_expressao() );

                    if( currentToken()->token == "PA_THEN" )
                    {
                            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                            tokensPointer++;

                            retorno->insereFilho( this->cs_comandoSemRotulo() );

                            if( currentToken()->token == "PA_ELSE" )
                            {
                                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                                    tokensPointer++;

                                    retorno->insereFilho( this->cs_comandoSemRotulo() );
                            }
                    }
                    else
                    {
                            erro("Esperado 'then'" );
                    }
            }
            else
            {
                    erro("Esperado 'if'");
            }

            return retorno;
}
NoType * AnalizadorSintatico::sub_comandoRepetitivo()
{
    NoType* retorno = new NoType( "<comando_repetitivo>", this->nvLexico, false );

    if( currentToken()->token == "PA_WHILE" )
    {
            retorno->insereFilho( currentToken()->token, this->nvLexico, true );
            tokensPointer++;

            retorno->insereFilho( this->cs_expressao() );

            if( currentToken()->token == "PA_DO" )
            {
                    retorno->insereFilho( currentToken()->token, this->nvLexico, true );
                    tokensPointer++;

                    retorno->insereFilho( cs_comandoSemRotulo() );
            }
            else
            {
                    erro( "Esperado 'do'" );
            }
    }
    else
    {
            erro("Esperado 'while'" );
    }

    return retorno;

}

bool AnalizadorSintatico::oprRel(QString token)
{
    return  token=="OP_IGUAL"  ||
            token=="OP_DIFERE" ||
            token=="OP_MENOR"  ||
            token=="OP_MENORIG"||
            token=="OP_MAIOR"  ||
            token=="OP_MAIORIG";

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
