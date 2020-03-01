#  GCCC

O G Curvaceous Cuiabano Compiler é um compilador para linguagem G, que foi feito para a disciplina de compiladores I.

### G:

A gramática G possui a seguinte definição:

* G = ( {Z,I,D,L,X,K,O,S,E,R,T}, {var, : , id, , , integer, real, ; , :=, if, then,+}, P, Z)

  <details>
      <summary>Regras de produção:</summary>
      <p>
      	<ul>
              <li>Z → I S</li>
              <li>I → var D</li>
              <li>D → L : K O</li>
              <li>L → id X</li>
              <li>X → , L</li>
              <li>X → ε</li>
              <li>K → integer</li>
              <li>K → real</li>
              <li>O → ; D</li>
              <li>O →ε</li>
              <li>S → id := E</li>
              <li>S → if E then S</li>
              <li>E → T R</li>
              <li>R → + T R</li>
              <li>R → ε</li>
              <li>T → id</li>
  		<ul>
      </p>
  </details>

###  TODO:

- [x] 1. [Faça os grafos sintáticos de cada uma das regras de G.](grafosSintaticos.md) 

- [ ] 2. Implemente um analisador léxico para G.

     <details>
         <summary>Detalhes:</summary>
         <p>
             Entrada: arquivo texto, contento uma sequência de código
     		escrito na linguagem G.
         </p>
         <p>
       		Saída: lista de tokens de G ou erro léxico. 
         </p>
     </details>

- [ ] 3.  Implemente um analisador sintático descendente para analisar cadeias pertencentes à linguagem gerada por G.

     <details>
         <summary>Detalhes:</summary>
         <p>
             Utilize a lista de tokens gerada pelo analisador léxico construído
             na parte 2.
         </p>
         <p>
             Entrada: arquivo texto, contento uma sequência de código
             escrito na linguagem G.
             Saída: cadeia sintaticamente correta ou erro sintático (se erro,
             imprimir qual erro).
         </p>
     </details>

- [ ] 4. Acrescente, aos grafos sintáticos (parte 1), ações semânticas inserção dos identificadores na tabela de símbolos e de verificação de tipos ( tomar como base slides do material de análise semântica, principalmente o 16).

- [ ] 5.  Implemente a tabela de símbolos com rotinas de busca e inserção. Acrescente ao analisador sintático as ações semânticas que você definiu de busca e inserção na tabela de símbolos e de verificação de tipos.

- [ ] 6.  Acrescente aos grafos sintáticos e à sua implementação a geração de código intermediário, com regras tais como previstas abaixo:

     <details>
         <summary>Detalhes:</summary>
         <p>
             S → id := {E.esq = id.lexval} E {gera(‘:=‘, E.dir, ́ ́, E.esq)}<br/>
     		E → T {R.esq = T.dir} R {E.dir = R.dir}<br/>
     		R → + T {R<sub>1</sub>.esq = T.dir } R<sub>1</sub>{R.dir :=geratemp; gera(‘+’, R.esq, R<sub>1</sub>.dir, R.dir)}<br/>
     		R → ε {R.dir = R.esq}<br/>
     		T → id {T.dir = id.lexval}<br/>
     		S → if E then {S<sub>1</sub>.quad := prox; prox := prox+1} S<sub>1</sub>
     {remenda(S<sub>1</sub>.quad, JF, E.dir, prox, ‘ ‘)}
         </p>
         <p>
             Ps: O índice (<sub>1</sub>) serve apenas para diferenciar qual não terminal está sendo referido nas regras das ações semânticas. Na sintaxe, são os mesmos elementos. Exemplo:Na regra: R → + T R<sub>1</sub> a, os dois Rs são o mesmo.
         </p>
         <p>
             <table>
             	<thead>
                 	<th>Exemplo de entrada</th>
                     <th>Saída produzida</th>
                 </thead>
                 <tbody>
                     <tr>
                         <td>
                             <p>
                             	var<br/>
      								a, b, c: integer;<br/>
      								d:real<br/>
     								if a+b then a:=c<br/>
                             </p>
                         </td>
                         <td>
                             <p>
                                 1: [+ a b t1]<br/>
                                 2: [JF t1 4 -]<br/>
                                 3: [:= a c -]<br/>
                                 4: [ ... ] <br/>
                             </p>
                         </td>
                     </tr>
                 </tbody>
         	</table>
         </p>
     </details>

