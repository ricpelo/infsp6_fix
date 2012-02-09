! =====================================================================
! INFSP (Spanish Inform Library)
! =====================================================================
!        Author: INFSP Task Team
!       Version: 0.9
!      Released: 21- Enero - 2009
! Serial Number: 901.21
!          Note: For use with Inform 6. (Tested with Inform compiler
!                version 6.31 and Inform library 6/11).
!   Description: "An Inform library to write Interactive Fiction in
!                Spanish. INFSPR.h: Parsers Replacements/Hack."
! =====================================================================

 Message "Incluyendo INFSPR [INFSP 0.9]";
! Constant INFSPR_adv; !descomentar esta linea si se quiere tener los mensajes de 
                       ! reemplazo de rutinas hackeadas al compilar

! ------------------------------------
! Parser Replace Section
! ------------------------------------
! Rutinas Hackeadas:
!                   DictionaryLookup		BUG
!                   BestGuess                   SP PATCH        
!                   Identical                       SP PATCH        
!                   PrefaceByArticle           SP PATCH        
!                   TryGivenObject            SP PATCH        
!                   NounDomain                SP PATCH        
!                   Indefart                       SP PATCH        


! Definicion de bufferaux y parseraux, usados en DictionaryLookup [001115]
#ifdef TARGET_ZCODE;
  Array bufferaux -> 123;
  Array parseaux  buffer 63;
#ifnot; ! TARGET_GLULX
  Array  bufferaux    buffer INPUT_BUFFER_LEN;
  Array parseaux  --> PARSE_BUFFER_LEN;
#endif; ! TARGET_



! Tremendo bug corregido. Antes usaba el buffer buf2, el cual usa
! tambien el parser para leer commandos incompletos del estilo de "¿a
! cual te refieres exactamente?"
! Este bug estaba en la libreria original, pero nunca aparecia porque
! apenas se usa DictionaryLookup. En cambio InformATE lo usa para
! comprobar si quitando la R a un verbo se obtiene una palabra válida.
!
! [Gracias a Presi por detectarlo]
!
#ifdef INFSPR_adv; Message "   Incluyendo reemplazo DictionaryLookup"; #endif;

[ DictionaryLookup texto longitud
    i;

 ! copiar texto a bufferaux (copiado especial)
 for (i=0:i<longitud:i++) bufferaux->(WORDSIZE+i) = texto->i;

 ! completar correctamente cabeceras de los buffers
 !	para la rutina Tokenise.
#ifdef TARGET_ZCODE;
  bufferaux->1 = longitud;
!  bufferaux->0 = longitud;
  bufferaux->0 = 120;
  parseaux-> 0 = 1;
#ifnot; ! TARGET_GLULX
  bufferaux-->0 = longitud;
#endif; ! TARGET_

            if (parser_trace>=9) { ! infsp debug
                print "    DictionaryLookup: texto a procesar: |";
                  ImprimeTodoElBuffer(bufferaux);
                print "|^";
            }
  Tokenise__(bufferaux,parseaux); ! procesar texto de bufferaux, el resultado está en parseaux-->1
  return parseaux-->1; ! retornar address del token encontrado (-1 si falla)
];


! ###############################################################################
! ----------------------------------------------------------------------------
!  BestGuess makes the best guess it can out of the match list, assuming
!  that everything in the match list is textually as good as everything else;
!  however it ignores items marked as -1, and so marks anything it chooses.
!  It returns -1 if there are no possible choices.
! ----------------------------------------------------------------------------

! Modificada en la revisión [020415] de modo que si hay varios objetos con la
! mayor puntuación, se elija uno de ellos al azar, en vez de elegir siempre al
! primero. Solo usado en I6. I7 usa uno mas elaborado.
#ifdef INFSPR_adv;Message "   Incluyendo reemplazo BestGuess"; #endif;
[ BestGuess  earliest its_score best i mejores aleat j;

  earliest=0; best=-1; mejores=0;
  for (i=0:i<number_matched:i++)
  {   if (match_list-->i >= 0)
      {   its_score=match_scores-->i;
          if (its_score>best) {
            best=its_score;
            earliest=i;
            mejores=1;
          }
          else if (its_score == best)
            mejores++;
      }
  }
  if (mejores > 1) {
    aleat = random (mejores);
    for (i = 0, j = 0: i < number_matched: i++)
    ! Antes se initializaba j = 1 (bug corregido en [020423])
    {
      if (match_list-->i >= 0)
      {
        its_score = match_scores-->i;
        if (its_score == best)
          j++;
        if (j == aleat)
        {
          earliest = i;
          break;
        }
      }
    }
  }
#ifdef DEBUG;
  if (parser_trace>=4)
  {   
      if (best < 0) print "   Best guess ran out of choices^";
      else print "   Best guess ", (the) match_list-->earliest, " (", match_list-->earliest, ")^";
  }
#endif;
  if (best<0) return -1;
  i=match_list-->earliest;
  match_list-->earliest=-1;
  bestguess_score = best;
  return i;
];

! ##############################################################################
! ----------------------------------------------------------------------------
!  Identical decides whether or not two objects can be distinguished from
!  each other by anything the player can type.  If not, it returns true.
! ----------------------------------------------------------------------------

! Modificado en la versión [020423] para comparar además de name, las otras
! propiedades de la librería española InformATE! que sirven también para nombrar
! los objetos: name_f, name_fp, name_mp y adjectives. Si se programan bien
! los juegos, con esto se puede solucionar el problema de la desambiguación.
#ifdef INFSPR_adv;Message "   Incluyendo reemplazo Identical"; #endif;
[ Identical o1 o2 p1 p2 n1 n2 i j flag pasada;

  if (o1==o2) rtrue;  ! This should never happen, but to be on the safe side
  if (o1==0 || o2==0) rfalse;  ! Yesmilarly
  if (parent(o1)==compass || parent(o2)==compass) rfalse; ! Saves time

!  What complicates things is that o1 or o2 might have a parsing routine,
!  so the parser can't know from here whether they are or aren't the same.
!  If they have different parsing routines, we simply assume they're
!  different.  If they have the same routine (which they probably got from
!  a class definition) then the decision process is as follows:
!
!     the routine is called (with self being o1, not that it matters)
!       with noun and second being set to o1 and o2, and action being set
!       to the fake action TheSame.  If it returns -1, they are found
!       identical; if -2, different; and if >=0, then the usual method
!       is used instead.

  if (o1.parse_name~=0 || o2.parse_name~=0) !ambos tienen propiedad parser_name?
  {   if (o1.parse_name ~= o2.parse_name) rfalse;
      parser_action=##TheSame; parser_one=o1; parser_two=o2;
      j=wn; i=RunRoutines(o1,parse_name); wn=j;
      if (i==-1) rtrue; if (i==-2) rfalse;
  }

!  This is the default algorithm: do they have the same words in their
!  "name" (i.e. property no. 1) properties.  (Note that the following allows
!  for repeated words and words in different orders.)

!  p1 = o1.&1; n1 = (o1.#1)/WORDSIZE;  ! Para mí, referirse a una propiedad
!  p2 = o2.&1; n2 = (o2.#1)/WORDSIZE;  ! con un número es una burrada

! ¿TODO? OJO: Este método compara si cada una de las propiedades de names y
! demás a buscar tiene las mismas palabras, pero entre ellas mismas (esto es,
! por ejemplo entre adjetivos de o1 y de o2), no entre sí (como, por ejemplo,
! entre las de name de o1 y adjetivos de o2).
!
! Posiblemente lo ideal fuera mirar si todas las palabras de todas estas
! propiedades de o1 están en las propiedades de o2 y viceversa.

! pasada almacenará el número de pasada por el que se va, para saber si toca
! comparar name(1), adjetivos(2), name_f(3), name_mp(4) o name_fp(5)  

  for (pasada = 1: pasada < 6: pasada++) {
    switch (pasada) {
      1:
        p1 = o1.&name; n1 = (o1.#name)/WORDSIZE;
        p2 = o2.&name; n2 = (o2.#name)/WORDSIZE;
      2:
        p1 = o1.&adjectives; n1 = (o1.#adjectives)/WORDSIZE;
        p2 = o2.&adjectives; n2 = (o2.#adjectives)/WORDSIZE;
      3:
        p1 = o1.&name_f; n1 = (o1.#name_f)/WORDSIZE;
        p2 = o2.&name_f; n2 = (o2.#name_f)/WORDSIZE;
      4:
        p1 = o1.&name_mp; n1 = (o1.#name_mp)/WORDSIZE;
        p2 = o2.&name_mp; n2 = (o2.#name_mp)/WORDSIZE;
      5:
        p1 = o1.&name_fp; n1 = (o1.#name_fp)/WORDSIZE;
        p2 = o2.&name_fp; n2 = (o2.#name_fp)/WORDSIZE;
    }  ! del switch

    !  for (i=0:i<n1:i++) { print (address) p1-->i, " "; } new_line;
    !  for (i=0:i<n2:i++) { print (address) p2-->i, " "; } new_line;

    for (i = 0: i < n1: i++)
    {
      flag = 0;
      for (j = 0: j < n2: j++)
        if (p1-->i == p2-->j)
          flag = 1;
      if (flag == 0) rfalse;
    }

    for (j = 0: j < n2: j++)
    {
      flag = 0;
      for (i = 0: i < n1: i++)
        if (p1-->i == p2-->j)
          flag = 1;
      if (flag == 0) rfalse;
    }
  }  ! del for

!  print "Which are identical!^";
  rtrue;
];


! ##############################################################################
! PrefaceByArticle actualizado a la 6/11, con hackeos de genero (a partir de INFSP0.8f)

#ifdef INFSPR_adv;Message "   Incluyendo reemplazo PrefaceByArticle"; #endif;
[ PrefaceByArticle o acode pluralise capitalise  i artform findout artval;
    if (o provides articles) {
        artval=(o.&articles)-->(acode+short_name_case*LanguageCases);
        if (artval) { ! infsp hack para admitir valor 0 en slot de 'articles' property.
         if (capitalise)
            print (Cap) artval, " ";
         else
            print (string) artval, " ";
        }
        if (pluralise) return;
        print (PSN__) o; return;
    }

   if (o provides gender){ ![infsp]'gender'(informATE exclusive) es el genero de short_name del objeto
    i=o.gender;
    if (i==1) ! infsp : esto es una chanchada, pero al cambiarse los valores de 'gender' por lo de la 
       i=0;   ! compatibilidad con I7 (donde no puede ponerse gender = 0), quedo desfasado el array
    if (i==2) ! LanguageGNAsToArticles. TODO: correjir el array y testear estabilidad.
       i=1;
   }else{ i = GetGNAOfObject(o);}

!    print "^pluralise:",pluralise; ![infsp] debug
!    print "^1 valor de i:",i; ![infsp] debug

! [infsp]: para q funcione con (el_), se agrego '>0' para el correcto funcionamiento (sino al final imprime el nombre de objeto cuando yo solo quiero imprimir su articulo)
    if (pluralise>0) { ![INFSP] ej, en ListMiscellany 19,[TODO] esto podria resolverse como los italianos
        if (i < 3 || (i >= 6 && i < 9)) i = i + 3;
    }
!    print "^2 valor de i:",i; ![infsp] debug
    i = LanguageGNAsToArticles-->i;
!    print "^3 valor de i:",i," ";! infsp debug
    
    artform = LanguageArticles
        + 3*WORDSIZE*LanguageContractionForms*(short_name_case + i*LanguageCases);

    #Iftrue (LanguageContractionForms == 2);
    if (artform-->acode ~= artform-->(acode+3)) findout = true;
    #Endif; ! LanguageContractionForms
    #Iftrue (LanguageContractionForms == 3);
    if (artform-->acode ~= artform-->(acode+3)) findout = true;
    if (artform-->(acode+3) ~= artform-->(acode+6)) findout = true;
    #Endif; ! LanguageContractionForms
    #Iftrue (LanguageContractionForms == 4);
    if (artform-->acode ~= artform-->(acode+3)) findout = true;
    if (artform-->(acode+3) ~= artform-->(acode+6)) findout = true;
    if (artform-->(acode+6) ~= artform-->(acode+9)) findout = true;
    #Endif; ! LanguageContractionForms
    #Iftrue (LanguageContractionForms > 4);
    findout = true;
    #Endif; ! LanguageContractionForms

    #Ifdef TARGET_ZCODE;
    if (standard_interpreter ~= 0 && findout) {
        StorageForShortName-->0 = 160;
        @output_stream 3 StorageForShortName;
        if (pluralise) print (number) pluralise; else print (PSN__) o;
        @output_stream -3;
        acode = acode + 3*LanguageContraction(StorageForShortName + 2);
    }
    #Ifnot; ! TARGET_GLULX
    if (findout) {
        if (pluralise)
            PrintAnyToArray(StorageForShortName, 160, EnglishNumber, pluralise);
        else
            PrintAnyToArray(StorageForShortName, 160, PSN__, o);
        acode = acode + 3*LanguageContraction(StorageForShortName);
    }
    #Endif; ! TARGET_

    Cap (artform-->acode, ~~capitalise); ! print article
    if (pluralise) return;
    print (PSN__) o;!imprime el nombre del objeto
];
! #############################################################################
! ----------------------------------------------------------------------------
!  TryGivenObject tries to match as many words as possible in what has been
!  typed to the given object, obj.  If it manages any words matched at all,
!  it calls MakeMatch to say so, then returns the number of words (or 1
!  if it was a match because of inadequate input).
! ----------------------------------------------------------------------------
! INFSP: Se resetea a 'nombreusado' - relacionado con el genero del objeto.
#ifdef INFSPR_adv;Message "   Incluyendo reemplazo TryGivenObject"; #endif;
[ TryGivenObject obj nomatch threshold k w j;
    #Ifdef DEBUG;
    if (parser_trace >= 5) print "    Trying ", (the) obj, " (", obj, ") at word ", wn, "^";
    #Endif; ! DEBUG

    if (nomatch && obj == 0) return 0;

! if (nomatch) print "*** TryGivenObject *** on ", (the) obj, " at wn = ", wn, "^";

    dict_flags_of_noun = 0;

!  If input has run out then always match, with only quality 0 (this saves
!  time).

    if (wn > num_words) {
        if (nomatch) return 0;
        if (indef_mode ~= 0)
            dict_flags_of_noun = $$01110000;  ! Reject "plural" bit
        MakeMatch(obj,0);

        give obj ~nombreusado; ![INFSP] hack: evitar que un objeto no mencionado en este turno sea elejido (Related: ChooseObjects/ElijeObjetos)

        #Ifdef DEBUG;
        if (parser_trace >= 5) print "    Matched (0)^";
        #Endif; ! DEBUG
        return 1;
    }

!  Ask the object to parse itself if necessary, sitting up and taking notice
!  if it says the plural was used:

    if (obj.parse_name~=0) {
!       print " <obj ",(the) obj," has parse_name>^";! infsp debug
        parser_action = NULL; j=wn;
        k = RunRoutines(obj,parse_name);
!       print "     k value-->",k ,"^";! infsp debug

        if (k > 0) {
            
            give obj nombreusado; ! infsp hack, I7: cuando coinciden en el parse_name
                                  !   sino no pueden usarse adjetivos para desambiguar
            wn=j+k;

          .MMbyPN;

            if (parser_action == ##PluralFound)
                dict_flags_of_noun = dict_flags_of_noun | 4;

            if (dict_flags_of_noun & 4) {
                if (~~allow_plurals) k = 0;
                else {
                    if (indef_mode == 0) {
                        indef_mode = 1; indef_type = 0; indef_wanted = 0;
                    }
                    indef_type = indef_type | PLURAL_BIT;
                    if (indef_wanted == 0) indef_wanted = 100;
                }
            }

            #Ifdef DEBUG;
            if (parser_trace >= 5) print "    Matched (", k, ")^";
            #Endif; ! DEBUG
            if (nomatch == false) MakeMatch(obj,k);
            return k;
        }
        if (k == 0) jump NoWordsMatch;
    }

    ! The default algorithm is simply to count up how many words pass the
    ! Refers test:

    parser_action = NULL;

    w = NounWord();

    if (w == 1 && player == obj) { k=1; jump MMbyPN; }

    if (w >= 2 && w < 128 && (LanguagePronouns-->w == obj)) { k = 1; jump MMbyPN; }

    j=--wn;
    threshold = ParseNoun(obj);
    #Ifdef DEBUG;
    if (threshold >= 0 && parser_trace >= 5) print "    ParseNoun returned ", threshold, "^";
    #Endif; ! DEBUG
    if (threshold < 0) wn++;
    if (threshold > 0) { k = threshold; jump MMbyPN; }

    if (threshold == 0 || Refers(obj,wn-1) == 0) {
      .NoWordsMatch;
        if (indef_mode ~= 0) {
            k = 0; parser_action = NULL;
            jump MMbyPN;
        }
        rfalse;
    }

    if (threshold < 0) {
        threshold = 1;
        dict_flags_of_noun = (w->#dict_par1) & $$01110100;
        w = NextWord();
        while (Refers(obj, wn-1)) {
            threshold++;
            if (w)
               dict_flags_of_noun = dict_flags_of_noun | ((w->#dict_par1) & $$01110100);
            w = NextWord();
        }
    }

    k = threshold;
    jump MMbyPN;
];


! ###########################################################################################
! Esta version de NounDomain para I6 utiliza ImprimirListaDudosos (indirectamente via Mensajes 45,46)
! que es mas eficiente al listar solo los objetos realmente relevantes (uso de adjetivos y demas)

#ifdef INFSPR_adv;Message "   Incluyendo reemplazo NounDomain"; #endif;
[ NounDomain	domain1 domain2 context    first_word i j k l
                                        answer_words marker;
    marker = marker; ! [infsp] para evitar el warning de variable no usada.
                     !         no se usa en I6 pero por ahora sí en I7
    #Ifdef DEBUG;
    if (parser_trace >= 4) {
        print "   [NounDomain called at word ", wn, "^";
        print "   ";
        if (indef_mode) {
            print "seeking indefinite object: ";
            if (indef_type & OTHER_BIT)  print "other ";
            if (indef_type & MY_BIT)     print "my ";
            if (indef_type & THAT_BIT)   print "that ";
            if (indef_type & PLURAL_BIT) print "plural ";
            if (indef_type & LIT_BIT)    print "lit ";
            if (indef_type & UNLIT_BIT)  print "unlit ";
            if (indef_owner ~= 0) print "owner:", (name) indef_owner;
            new_line;
            print "   number wanted: ";
            if (indef_wanted == 100) print "all"; else print indef_wanted;
            new_line;
            print "   most likely GNAs of names: ", indef_cases, "^";
        }
        else print "seeking definite object^";
    }
    #Endif; ! DEBUG

    match_length = 0; number_matched = 0; match_from = wn; placed_in_flag = 0;

    SearchScope(domain1, domain2, context);

    #Ifdef DEBUG;
    if (parser_trace >= 4) print "   [ND made ", number_matched, " matches]^";
    #Endif; ! DEBUG

    wn = match_from+match_length;

    ! If nothing worked at all, leave with the word marker skipped past the
    ! first unmatched word...

    if (number_matched == 0) { wn++; rfalse; }

    ! Suppose that there really were some words being parsed (i.e., we did
    ! not just infer).  If so, and if there was only one match, it must be
    ! right and we return it...

    if (match_from <= num_words) {
        if (number_matched == 1) {
            i=match_list-->0;
            return i;
        }

        ! ...now suppose that there was more typing to come, i.e. suppose that
        ! the user entered something beyond this noun.  If nothing ought to follow,
        ! then there must be a mistake, (unless what does follow is just a full
        ! stop, and or comma)

        if (wn <= num_words) {
            i = NextWord(); wn--;
            if (i ~=  AND1__WD or AND2__WD or AND3__WD or comma_word
                   or THEN1__WD or THEN2__WD or THEN3__WD
                   or BUT1__WD or BUT2__WD or BUT3__WD) {
                if (lookahead == ENDIT_TOKEN) rfalse;
            }
        }
    }

    ! Now look for a good choice, if there's more than one choice...

    number_of_classes = 0;

    if (number_matched == 1) i = match_list-->0;
    if (number_matched > 1) {
        i = Adjudicate(context);
        if (i == -1) rfalse;
        if (i == 1) rtrue;       !  Adjudicate has made a multiple
                             !  object, and we pass it on
    }

    ! If i is non-zero here, one of two things is happening: either
    ! (a) an inference has been successfully made that object i is
    !     the intended one from the user's specification, or
    ! (b) the user finished typing some time ago, but we've decided
    !     on i because it's the only possible choice.
    ! In either case we have to keep the pattern up to date,
    ! note that an inference has been made and return.
    ! (Except, we don't note which of a pile of identical objects.)

    if (i ~= 0) {
        if (dont_infer) return i;
        if (inferfrom == 0) inferfrom=pcount;
        pattern-->pcount = i;
        return i;
    }

    ! If we get here, there was no obvious choice of object to make.  If in
    ! fact we've already gone past the end of the player's typing (which
    ! means the match list must contain every object in scope, regardless
    ! of its name), then it's foolish to give an enormous list to choose
    ! from - instead we go and ask a more suitable question...

    if (match_from > num_words) jump Incomplete;

    ! Now we print up the question, using the equivalence classes as worked
    ! out by Adjudicate() so as not to repeat ourselves on plural objects...

    if (context==CREATURE_TOKEN) L__M(##Miscellany, 45);
    else                         L__M(##Miscellany, 46);

    L__M(##Miscellany, 57); !imprime "?^"

    ! ...and get an answer:

  .WhichOne;
    #Ifdef TARGET_ZCODE;
    for (i=2 : i<INPUT_BUFFER_LEN : i++) buffer2->i = ' ';
    #Endif; ! TARGET_ZCODE
    answer_words=Keyboard(buffer2, parse2);

    ! Conveniently, parse2-->1 is the first word in both ZCODE and GLULX.
    first_word = (parse2-->1);

    ! Take care of "all", because that does something too clever here to do
    ! later on:

    if (first_word == ALL1__WD or ALL2__WD or ALL3__WD or ALL4__WD or ALL5__WD) {
        if (context == MULTI_TOKEN or MULTIHELD_TOKEN or MULTIEXCEPT_TOKEN or MULTIINSIDE_TOKEN) {
            l = multiple_object-->0;
            for (i=0 : i<number_matched && l+i<63 : i++) {
                k = match_list-->i;
                multiple_object-->(i+1+l) = k;
            }
            multiple_object-->0 = i+l;
            rtrue;
        }
        L__M(##Miscellany, 47);
        jump WhichOne;
    }

    ! If the first word of the reply can be interpreted as a verb, then
    ! assume that the player has ignored the question and given a new
    ! command altogether.
    ! (This is one time when it's convenient that the directions are
    ! not themselves verbs - thus, "north" as a reply to "Which, the north
    ! or south door" is not treated as a fresh command but as an answer.)

    #Ifdef LanguageIsVerb;
    if (first_word == 0) {
!    print "   ND:LanguageIsVerb: WhichOne section^"; ! infsp debug
        j = wn; first_word = LanguageIsVerb(buffer2, parse2, 1); wn = j;
    }
    #Endif; ! LanguageIsVerb
    if (first_word ~= 0) {
        j = first_word->#dict_par1;
        if ((0 ~= j&1) && ~~LanguageVerbMayBeName(first_word)) {
            if (parser_trace>=9) { ! [INFSP] for debugging proposes
                print "Copiando buffer2: |";
                    ImprimeTodoElBuffer(buffer2);
                print "|^   en buffer: |";
                    ImprimeTodoElBuffer(buffer);
                print "|^";
            
            }
            CopyBuffer(buffer, buffer2);
            jump RECONSTRUCT_INPUT;
        }
    }

    ! Now we insert the answer into the original typed command, as
    ! words additionally describing the same object
    ! (eg, > take red button
    !      Which one, ...
    !      > music
    ! becomes "take music red button".  The parser will thus have three
    ! words to work from next time, not two.)

    #Ifdef TARGET_ZCODE;
    k = WordAddress(match_from) - buffer; l=buffer2->1+1;
    for (j=buffer + buffer->0 - 1 : j>=buffer+k+l : j--) j->0 = 0->(j-l);
    for (i=0 : i<l : i++) buffer->(k+i) = buffer2->(2+i);
    buffer->(k+l-1) = ' ';
    buffer->1 = buffer->1 + l;
    if (buffer->1 >= (buffer->0 - 1)) buffer->1 = buffer->0;
    #Ifnot; ! TARGET_GLULX
    k = WordAddress(match_from) - buffer;
    l = (buffer2-->0) + 1;
    for (j=buffer+INPUT_BUFFER_LEN-1 : j>=buffer+k+l : j--) j->0 = j->(-l);
    for (i=0 : i<l : i++) buffer->(k+i) = buffer2->(WORDSIZE+i);
    buffer->(k+l-1) = ' ';
    buffer-->0 = buffer-->0 + l;
    if (buffer-->0 > (INPUT_BUFFER_LEN-WORDSIZE)) buffer-->0 = (INPUT_BUFFER_LEN-WORDSIZE);
    #Endif; ! TARGET_

    ! Having reconstructed the input, we warn the parser accordingly
    ! and get out.

    .RECONSTRUCT_INPUT;

    return REPARSE_CODE;

    ! Now we come to the question asked when the input has run out
    ! and can't easily be guessed (eg, the player typed "take" and there
    ! were plenty of things which might have been meant).

  .Incomplete;

    if (context == CREATURE_TOKEN) L__M(##Miscellany, 48);
    else                           L__M(##Miscellany, 49);

    #Ifdef TARGET_ZCODE;
    for (i=2 : i<INPUT_BUFFER_LEN : i++) buffer2->i=' ';
    #Endif; ! TARGET_ZCODE
    answer_words = Keyboard(buffer2, parse2);

    first_word=(parse2-->1);
    #Ifdef LanguageIsVerb;
    if (first_word==0) {
!    print "   ND:LanguageIsVerb: Incomplete section^"; ! infsp debug
        j = wn; first_word=LanguageIsVerb(buffer2, parse2, 1); wn = j;
    }
    #Endif; ! LanguageIsVerb

    ! Once again, if the reply looks like a command, give it to the
    ! parser to get on with and forget about the question...

    if (first_word ~= 0) {
        j = first_word->#dict_par1;
        if (0 ~= j&1) {
            CopyBuffer(buffer, buffer2);
            return REPARSE_CODE;
        }
    }

    ! ...but if we have a genuine answer, then:
    !
    ! (1) we must glue in text suitable for anything that's been inferred.

    if (inferfrom ~= 0) {
        for (j=inferfrom : j<pcount : j++) {
            if (pattern-->j == PATTERN_NULL) continue;
            #Ifdef TARGET_ZCODE;
            i = 2+buffer->1; (buffer->1)++; buffer->(i++) = ' ';
            #Ifnot; ! TARGET_GLULX
            i = WORDSIZE + buffer-->0;
            (buffer-->0)++; buffer->(i++) = ' ';
            #Endif; ! TARGET_

            #Ifdef DEBUG;
            if (parser_trace >= 5) print "[Gluing in inference with pattern code ", pattern-->j, "]^";
            #Endif; ! DEBUG

            ! Conveniently, parse2-->1 is the first word in both ZCODE and GLULX.

            parse2-->1 = 0;

            ! An inferred object.  Best we can do is glue in a pronoun.
            ! (This is imperfect, but it's very seldom needed anyway.)

            if (pattern-->j >= 2 && pattern-->j < REPARSE_CODE) {
                PronounNotice(pattern-->j);
                for (k=1 : k<=LanguagePronouns-->0 : k=k+3)
                    if (pattern-->j == LanguagePronouns-->(k+2)) {
                        parse2-->1 = LanguagePronouns-->k;
                        #Ifdef DEBUG;
                        if (parser_trace >= 5) print "[Using pronoun '", (address) parse2-->1, "']^";
                        #Endif; ! DEBUG
                        break;
                    }
            }
            else {
                ! An inferred preposition.
                parse2-->1 = No__Dword(pattern-->j - REPARSE_CODE);
                #Ifdef DEBUG;
                if (parser_trace >= 5) print "[Using preposition '", (address) parse2-->1, "']^";
                #Endif; ! DEBUG
            }

            ! parse2-->1 now holds the dictionary address of the word to glue in.

            if (parse2-->1 ~= 0) {
                k = buffer + i;
                #Ifdef TARGET_ZCODE;
                @output_stream 3 k;
                 print (address) parse2-->1;
                @output_stream -3;
                k = k-->0;
                for (l=i : l<i+k : l++) buffer->l = buffer->(l+2);
                i = i + k; buffer->1 = i-2;
                #Ifnot; ! TARGET_GLULX
                k = PrintAnyToArray(buffer+i, INPUT_BUFFER_LEN-i, parse2-->1);
                i = i + k; buffer-->0 = i - WORDSIZE;
                #Endif; ! TARGET_
            }
        }
    }

    ! (2) we must glue the newly-typed text onto the end.

    #Ifdef TARGET_ZCODE;
    i = 2+buffer->1; (buffer->1)++; buffer->(i++) = ' ';
    for (j=0 : j<buffer2->1 : i++,j++) {
        buffer->i = buffer2->(j+2);
        (buffer->1)++;
        if (buffer->1 == INPUT_BUFFER_LEN) break;
    }
    #Ifnot; ! TARGET_GLULX
    i = WORDSIZE + buffer-->0;
    (buffer-->0)++; buffer->(i++) = ' ';
    for (j=0 : j<buffer2-->0 : i++,j++) {
        buffer->i = buffer2->(j+WORDSIZE);
        (buffer-->0)++;
        if (buffer-->0 == INPUT_BUFFER_LEN) break;
    }
    #Endif; ! TARGET_

    ! (3) we fill up the buffer with spaces, which is unnecessary, but may
    !     help incorrectly-written interpreters to cope.

    #Ifdef TARGET_ZCODE;
    for (: i<INPUT_BUFFER_LEN : i++) buffer->i = ' ';
    #Endif; ! TARGET_ZCODE

    return REPARSE_CODE;

]; ! end of NounDomain


! #############################################################################
! [infsp] Indefart: se agregó el "a " en caso de nombre propio " a Mamá"
#ifdef INFSPR_adv;Message "   Incluyendo reemplazo Indefart"; #endif;
[ Indefart o i; ! funcion (a)
    if (o == 0) { print (string) NOTHING__TX; rtrue; }
    i = indef_mode; indef_mode = true;
    if (o has proper) { indef_mode = NULL; print "a ",(PSN__) o; indef_mode = i; return; }
    if (o provides article) {
        PrintOrRun(o, article, 1); print " ", (PSN__) o; indef_mode = i;
        return;
    }
    PrefaceByArticle(o, 2); indef_mode = i;
];



! ------------------------------------
! Verlib Replace Section
! ------------------------------------

! Nada por ahora

! Fin del archivo INFSPR
