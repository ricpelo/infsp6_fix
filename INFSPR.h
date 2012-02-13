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
!                   DictionaryLookup          BUG
!                   BestGuess                 SP PATCH
!                   Identical                 SP PATCH
!                   PrefaceByArticle          SP PATCH
!                   TryGivenObject            SP PATCH
!                   NounDomain                SP PATCH
!                   Indefart                  SP PATCH
!                   CInDefArt                 SP PATCH
!                   Parser__parse             SP PATCH
!                   ReviseMulti               SP PATCH
!                   ChangePlayer              SP PATCH


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
    if (i==1)      ! infsp : esto es una chanchada, pero al cambiarse los valores de 'gender' por lo de la 
       i=0;        ! compatibilidad con I7 (donde no puede ponerse gender = 0), quedo desfasado el array
    else if (i==2) ! LanguageGNAsToArticles. TODO: corregir el array y testear estabilidad.
       i=1;        !
    else if (i==4 or 5) ! Todos estos los incluyo yo (Sothoth)...
       i=3;             ! para ajustar el array...
    else if (i==6)      ! al nuevo valor...
       i=4;             ! de G_PLURAL
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
        wn = j;
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

#ifdef INFSPR_adv;Message "   Incluyendo reemplazo CInDefArt"; #endif;
[ CInDefArt o i;
    i = indef_mode; indef_mode = true;
    if (o has proper) { indef_mode = NULL; print "a ",(PSN__) o; indef_mode = i; return; }
    if (o provides article) {
        PrintCapitalised(o, article, 1); print " ", (PSN__) o; indef_mode = i;
        return;
    }
    PrefaceByArticle(o, 2, 0, 1); indef_mode = i;
];

! #############################################################################
#ifdef INFSPR_adv;Message "   Incluyendo reemplazo Parser__parse"; #endif;
[ Parser__parse  results   syntax line num_lines line_address i j k
                           token l m;

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! A: Get the input, do "oops" and "again"
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ! Firstly, in "not held" mode, we still have a command left over from last
    ! time (eg, the user typed "eat biscuit", which was parsed as "take biscuit"
    ! last time, with "eat biscuit" tucked away until now).  So we return that.

    if (notheld_mode == 1) {
        for (i=0 : i<8 : i++) results-->i = kept_results-->i;
        notheld_mode = 0;
        rtrue;
    }

    if (held_back_mode == 1) {
        held_back_mode = 0;
        Tokenise__(buffer, parse);
        jump ReParse;
    }

  .ReType;

    Keyboard(buffer,parse);

  .ReParse;

    parser_inflection = name;

    ! Initially assume the command is aimed at the player, and the verb
    ! is the first word

    #Ifdef TARGET_ZCODE;
    num_words = parse->1;
    #Ifnot; ! TARGET_GLULX
    num_words = parse-->0;
    #Endif; ! TARGET_
    wn = 1;

    #Ifdef LanguageToInformese;
    LanguageToInformese();
    #IfV5;
    ! Re-tokenise:
    Tokenise__(buffer,parse);
    #Endif; ! V5
    #Endif; ! LanguageToInformese

    BeforeParsing();
    #Ifdef TARGET_ZCODE;
    num_words = parse->1;
    #Ifnot; ! TARGET_GLULX
    num_words = parse-->0;
    #Endif; ! TARGET_

    k=0;
    #Ifdef DEBUG;
    if (parser_trace >= 2) {
        print "[ ";
        for (i=0 : i<num_words : i++) {

            #Ifdef TARGET_ZCODE;
            j = parse-->(i*2 + 1);
            #Ifnot; ! TARGET_GLULX
            j = parse-->(i*3 + 1);
            #Endif; ! TARGET_
            k = WordAddress(i+1);
            l = WordLength(i+1);
            print "~"; for (m=0 : m<l : m++) print (char) k->m; print "~ ";

            if (j == 0) print "?";
            else {
                #Ifdef TARGET_ZCODE;
                if (UnsignedCompare(j, HDR_DICTIONARY-->0) >= 0 &&
                    UnsignedCompare(j, HDR_HIGHMEMORY-->0) < 0)
                     print (address) j;
                else print j;
                #Ifnot; ! TARGET_GLULX
                if (j->0 == $60) print (address) j;
                else print j;
                #Endif; ! TARGET_
            }
            if (i ~= num_words-1) print " / ";
        }
        print " ]^";
    }
    #Endif; ! DEBUG
    verb_wordnum = 1;
    actor = player;
    actors_location = ScopeCeiling(player);
    usual_grammar_after = 0;

  .AlmostReParse;

    scope_token = 0;
    action_to_be = NULL;

    ! Begin from what we currently think is the verb word

  .BeginCommand;

    wn = verb_wordnum;
    verb_word = NextWordStopped();

    ! If there's no input here, we must have something like "person,".

    if (verb_word == -1) {
        best_etype = STUCK_PE;
        jump GiveError;
    }

    ! Now try for "again" or "g", which are special cases: don't allow "again" if nothing
    ! has previously been typed; simply copy the previous text across

    if (verb_word == AGAIN2__WD or AGAIN3__WD) verb_word = AGAIN1__WD;
    if (verb_word == AGAIN1__WD) {
        if (actor ~= player) {
            L__M(##Miscellany, 20);
            jump ReType;
        }
        #Ifdef TARGET_ZCODE;
        if (buffer3->1 == 0) {
            L__M(##Miscellany, 21);
            jump ReType;
        }
        #Ifnot; ! TARGET_GLULX
        if (buffer3-->0 == 0) {
            L__M(##Miscellany, 21);
            jump ReType;
        }
        #Endif; ! TARGET_
        if (WordAddress(verb_wordnum) == buffer + WORDSIZE) { ! not held back
            ! splice rest of buffer onto end of buffer3
            #Ifdef TARGET_ZCODE;
            i = buffer3->1;
            #Ifnot; ! TARGET_GLULX
            i = buffer3-->0;
            #Endif;
            while (buffer3->(i + WORDSIZE - 1) == ' ' or '.')
                i--;
            j = i - WordLength(verb_wordnum);  ! amount to move buffer up by
            if (j > 0) {
                for (m=INPUT_BUFFER_LEN-1 : m>=WORDSIZE+j : m--)
                    buffer->m = buffer->(m-j);
                #Ifdef TARGET_ZCODE;
                buffer->1 = buffer->1 + j;
                #Ifnot; ! TARGET_GLULX
                buffer-->0 = buffer-->0 + j;
                #Endif;
            }
            for (m=WORDSIZE : m<WORDSIZE+i : m++) buffer->m = buffer3->m;
            if (j < 0) for (:m<WORDSIZE+i-j : m++) buffer->m = ' ';
        } else
            for (i=0 : i<INPUT_BUFFER_LEN : i++) buffer->i = buffer3->i;
        jump ReParse;
    }

    ! Save the present input in case of an "again" next time

    if (verb_word ~= AGAIN1__WD)
        for (i=0 : i<INPUT_BUFFER_LEN : i++) buffer3->i = buffer->i;

    if (usual_grammar_after == 0) {
        j = verb_wordnum;
        i = RunRoutines(actor, grammar); 
        #Ifdef DEBUG;
        if (parser_trace >= 2 && actor.grammar ~= 0 or NULL)
            print " [Grammar property returned ", i, "]^";
        #Endif; ! DEBUG

        #Ifdef TARGET_ZCODE;
        if ((i ~= 0 or 1) &&
            (UnsignedCompare(i, dict_start) < 0 ||
             UnsignedCompare(i, dict_end) >= 0 ||
             (i - dict_start) % dict_entry_size ~= 0)) {
            usual_grammar_after = j;
            i=-i;
        }

        #Ifnot; ! TARGET_GLULX
        if (i < 0) { usual_grammar_after = verb_wordnum; i=-i; }
        #Endif;

        if (i == 1) {
            results-->0 = action;
            results-->1 = 2;  ! [VERGIT] Aqu<ED> va el n<FA>mero de par<E1>metros
            results-->2 = noun;
            results-->3 = second;
            rtrue;
        }
        if (i ~= 0) { verb_word = i; wn--; verb_wordnum--; }
        else { wn = verb_wordnum; verb_word = NextWord(); }
    }
    else usual_grammar_after = 0;

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! B: Is it a direction, and so an implicit "go"?  If so go to (K)
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    #Ifdef LanguageIsVerb;
    if (verb_word == 0) {
        i = wn; verb_word = LanguageIsVerb(buffer, parse, verb_wordnum);
        wn = i;
    }
    #Endif; ! LanguageIsVerb

    ! If the first word is not listed as a verb, it must be a direction
    ! or the name of someone to talk to

    if (verb_word == 0 || ((verb_word->#dict_par1) & 1) == 0) {

        ! So is the first word an object contained in the special object "compass"
        ! (i.e., a direction)?  This needs use of NounDomain, a routine which
        ! does the object matching, returning the object number, or 0 if none found,
        ! or REPARSE_CODE if it has restructured the parse table so the whole parse
        ! must be begun again...

        wn = verb_wordnum; indef_mode = false; token_filter = 0;
        l = NounDomain(compass, 0, 0);
        if (l == REPARSE_CODE) jump ReParse;

        ! If it is a direction, send back the results:
        ! action=GoSub, no of arguments=1, argument 1=the direction.

        if (l ~= 0) {
            results-->0 = ##Go;
            action_to_be = ##Go;
            results-->1 = 1;
            results-->2 = l;
            jump LookForMore;
        }

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! C: Is anyone being addressed?
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        ! Only check for a comma (a "someone, do something" command) if we are
        ! not already in the middle of one.  (This simplification stops us from
        ! worrying about "robot, wizard, you are an idiot", telling the robot to
        ! tell the wizard that she is an idiot.)

        if (actor == player) {
            for (j=2 : j<=num_words : j++) {
                i=NextWord();
                if (i == comma_word) jump Conversation;
            }
            verb_word = UnknownVerb(verb_word);
            if (verb_word ~= 0) jump VerbAccepted;
        }
        best_etype = VERB_PE;
        jump GiveError;

        ! NextWord nudges the word number wn on by one each time, so we've now
        ! advanced past a comma.  (A comma is a word all on its own in the table.)

      .Conversation;

        j = wn - 1;
        if (j == 1) {
            L__M(##Miscellany, 22);
            jump ReType;
        }

        ! Use NounDomain (in the context of "animate creature") to see if the
        ! words make sense as the name of someone held or nearby

        wn = 1; lookahead = HELD_TOKEN;
        scope_reason = TALKING_REASON;
        l = NounDomain(player,actors_location,6);
        scope_reason = PARSING_REASON;
        if (l == REPARSE_CODE) jump ReParse;
        if (l == 0) {
            L__M(##Miscellany, 23);
            jump ReType;
        }

      .Conversation2;

        ! The object addressed must at least be "talkable" if not actually "animate"
        ! (the distinction allows, for instance, a microphone to be spoken to,
        ! without the parser thinking that the microphone is human).

        if (l hasnt animate && l hasnt talkable) {
            L__M(##Miscellany, 24, l);
            jump ReType;
        }

        ! Check that there aren't any mystery words between the end of the person's
        ! name and the comma (eg, throw out "dwarf sdfgsdgs, go north").

        if (wn ~= j) {
            L__M(##Miscellany, 25);
            jump ReType;
        }

        ! The player has now successfully named someone.  Adjust "him", "her", "it":

        PronounNotice(l);

        ! Set the global variable "actor", adjust the number of the first word,
        ! and begin parsing again from there.

        verb_wordnum = j + 1;

        ! Stop things like "me, again":

        if (l == player) {
            wn = verb_wordnum;
            if (NextWordStopped() == AGAIN1__WD or AGAIN2__WD or AGAIN3__WD) {
                L__M(##Miscellany, 20);
                jump ReType;
            }
        }

        actor = l;
        actors_location = ScopeCeiling(l);
        #Ifdef DEBUG;
        if (parser_trace >= 1)
            print "[Actor is ", (the) actor, " in ", (name) actors_location, "]^";
        #Endif; ! DEBUG
        jump BeginCommand;

    } ! end of first-word-not-a-verb

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! D: Get the verb: try all the syntax lines for that verb
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  .VerbAccepted;

    ! We now definitely have a verb, not a direction, whether we got here by the
    ! "take ..." or "person, take ..." method.  Get the meta flag for this verb:

    meta = ((verb_word->#dict_par1) & 2)/2;

    ! You can't order other people to "full score" for you, and so on...

    if (meta == 1 && actor ~= player) {
        best_etype = VERB_PE;
        meta = 0;
        jump GiveError;
    }

    ! Now let i be the corresponding verb number, stored in the dictionary entry
    ! (in a peculiar 255-n fashion for traditional Infocom reasons)...

    i = $ff-(verb_word->#dict_par2);

    ! ...then look up the i-th entry in the verb table, whose address is at word
    ! 7 in the Z-machine (in the header), so as to get the address of the syntax
    ! table for the given verb...

    #Ifdef TARGET_ZCODE;
    syntax = (HDR_STATICMEMORY-->0)-->i;
    #Ifnot; ! TARGET_GLULX
    syntax = (#grammar_table)-->(i+1);
    #Endif; ! TARGET_

    ! ...and then see how many lines (ie, different patterns corresponding to the
    ! same verb) are stored in the parse table...

    num_lines = (syntax->0) - 1;

    ! ...and now go through them all, one by one.
    ! To prevent pronoun_word 0 being misunderstood,

    pronoun_word = NULL; pronoun_obj = NULL;

    #Ifdef DEBUG;
    if (parser_trace >= 1) print "[Parsing for the verb '", (address) verb_word, "' (", num_lines+1, " lines)]^";
    #Endif; ! DEBUG

    best_etype = STUCK_PE; nextbest_etype = STUCK_PE;
    multiflag = false;

    ! "best_etype" is the current failure-to-match error - it is by default
    ! the least informative one, "don't understand that sentence".
    ! "nextbest_etype" remembers the best alternative to having to ask a
    ! scope token for an error message (i.e., the best not counting ASKSCOPE_PE).
    ! multiflag is used here to prevent inappropriate MULTI_PE errors
    ! in addition to its unrelated duties passing information to action routines

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! E: Break down a syntax line into analysed tokens
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    line_address = syntax + 1;

    for (line=0 : line<=num_lines : line++) {

        for (i=0 : i<32 : i++) {
            line_token-->i = ENDIT_TOKEN;
            line_ttype-->i = ELEMENTARY_TT;
            line_tdata-->i = ENDIT_TOKEN;
        }

        ! Unpack the syntax line from Inform format into three arrays; ensure that
        ! the sequence of tokens ends in an ENDIT_TOKEN.

        line_address = UnpackGrammarLine(line_address);

        #Ifdef DEBUG;
        if (parser_trace >= 1) {
            if (parser_trace >= 2) new_line;
            print "[line ", line; DebugGrammarLine();
            print "]^";
        }
        #Endif; ! DEBUG

        ! We aren't in "not holding" or inferring modes, and haven't entered
        ! any parameters on the line yet, or any special numbers; the multiple
        ! object is still empty.

        not_holding = 0;
        inferfrom = 0;
        parameters = 0;
        nsns = 0; special_word = 0; special_number = 0;
        multiple_object-->0 = 0;
        multi_context = 0;
        etype = STUCK_PE;

        ! Put the word marker back to just after the verb

        wn = verb_wordnum+1;

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! F: Look ahead for advance warning for multiexcept/multiinside
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        ! There are two special cases where parsing a token now has to be
        ! affected by the result of parsing another token later, and these
        ! two cases (multiexcept and multiinside tokens) are helped by a quick
        ! look ahead, to work out the future token now.  We can only carry this
        ! out in the simple (but by far the most common) case:
        !
        !     multiexcept <one or more prepositions> noun
        !
        ! and similarly for multiinside.

        advance_warning = NULL; indef_mode = false;
        for (i=0,m=false,pcount=0 : line_token-->pcount ~= ENDIT_TOKEN : pcount++) {
            scope_token = 0;

            if (line_ttype-->pcount ~= PREPOSITION_TT) i++;

            if (line_ttype-->pcount == ELEMENTARY_TT) {
                if (line_tdata-->pcount == MULTI_TOKEN) m = true;
                ! [001115] Añadida la comprobación MULTIHELD_TOKEN
                ! para capturar correctamente el caso DEJA TODO
                if (line_tdata-->pcount == MULTIHELD_TOKEN) {
                    m = true;
                    indef_owner = actor;
                }
                if (line_tdata-->pcount == MULTIEXCEPT_TOKEN or MULTIINSIDE_TOKEN  && i == 1) {
                    ! First non-preposition is "multiexcept" or
                    ! "multiinside", so look ahead.

                    #Ifdef DEBUG;
                    if (parser_trace >= 2) print " [Trying look-ahead]^";
                    #Endif; ! DEBUG

                    ! We need this to be followed by 1 or more prepositions.

                    pcount++;
                    if (line_ttype-->pcount == PREPOSITION_TT) {
                        while (line_ttype-->pcount == PREPOSITION_TT) pcount++;

                        if ((line_ttype-->pcount == ELEMENTARY_TT) && (line_tdata-->pcount == NOUN_TOKEN)) {

                            ! Advance past the last preposition

                            while (wn < num_words) {
                                l=NextWord();
                                if ( l && (l->#dict_par1) &8 ) {   ! if preposition
                                    l = Descriptors();  ! skip past THE etc
                                    if (l~=0) etype=l;  ! don't allow multiple objects
                                    l = NounDomain(actors_location, actor, NOUN_TOKEN);
                                    #Ifdef DEBUG;
                                    if (parser_trace >= 2) {
                                        print " [Advanced to ~noun~ token: ";
                                        if (l == REPARSE_CODE) print "re-parse request]^";
                                        if (l == 1) print "but multiple found]^";
                                        if (l == 0) print "error ", etype, "]^";
                                        if (l >= 2) print (the) l, "]^";
                                    }
                                    #Endif; ! DEBUG
                                    if (l == REPARSE_CODE) jump ReParse;
                                    if (l >= 2) advance_warning = l;
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }

        ! Slightly different line-parsing rules will apply to "take multi", to
        ! prevent "take all" behaving correctly but misleadingly when there's
        ! nothing to take.

        take_all_rule = 0;
        if (m && params_wanted == 1 && action_to_be == ##Take)
            take_all_rule = 1;

        ! And now start again, properly, forearmed or not as the case may be.
        ! As a precaution, we clear all the variables again (they may have been
        ! disturbed by the call to NounDomain, which may have called outside
        ! code, which may have done anything!).

        not_holding = 0;
        inferfrom = 0;
        parameters = 0;
        nsns = 0; special_word = 0; special_number = 0;
        multiple_object-->0 = 0;
        etype = STUCK_PE;
        wn = verb_wordnum+1;

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! G: Parse each token in turn (calling ParseToken to do most of the work)
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        ! "Pattern" gradually accumulates what has been recognised so far,
        ! so that it may be reprinted by the parser later on

        for (pcount=1 : : pcount++) {
            pattern-->pcount = PATTERN_NULL; scope_token = 0;

            token = line_token-->(pcount-1);
            lookahead = line_token-->pcount;

            #Ifdef DEBUG;
            if (parser_trace >= 2)
                print " [line ", line, " token ", pcount, " word ", wn, " : ", (DebugToken) token,
                  "]^";
            #Endif; ! DEBUG

            if (token ~= ENDIT_TOKEN) {
                scope_reason = PARSING_REASON;
                parser_inflection = name;
                AnalyseToken(token);

                if (action_to_be == ##AskTo && found_ttype == ELEMENTARY_TT &&
                    found_tdata == TOPIC_TOKEN)
                {
                    l=inputobjs-->2;
                    wn--;
                    j = wn;
                    jump Conversation2;
                }

                l = ParseToken__(found_ttype, found_tdata, pcount-1, token);
                while (l<-200) l = ParseToken__(ELEMENTARY_TT, l + 256);
                scope_reason = PARSING_REASON;

                if (l == GPR_PREPOSITION) {
                    if (found_ttype~=PREPOSITION_TT && (found_ttype~=ELEMENTARY_TT ||
                        found_tdata~=TOPIC_TOKEN)) params_wanted--;
                    l = true;
                }
                else
                    if (l < 0) l = false;
                    else
                        if (l ~= GPR_REPARSE) {
                            if (l == GPR_NUMBER) {
                                if (nsns == 0) special_number1 = parsed_number;
                                else special_number2 = parsed_number;
                                nsns++; l = 1;
                            }
                            if (l == GPR_MULTIPLE) l = 0;
                            results-->(parameters+2) = l;
                            parameters++;
                            pattern-->pcount = l;
                            l = true;
                        }

                #Ifdef DEBUG;
                if (parser_trace >= 3) {
                    print "  [token resulted in ";
                    if (l == REPARSE_CODE) print "re-parse request]^";
                    if (l == 0) print "failure with error type ", etype, "]^";
                    if (l == 1) print "success]^";
                }
                #Endif; ! DEBUG

                if (l == REPARSE_CODE) jump ReParse;
                if (l == false) break;
            }
            else {

                ! If the player has entered enough already but there's still
                ! text to wade through: store the pattern away so as to be able to produce
                ! a decent error message if this turns out to be the best we ever manage,
                ! and in the mean time give up on this line

                ! However, if the superfluous text begins with a comma or "then" then
                ! take that to be the start of another instruction

                if (wn <= num_words) {
                    l = NextWord();
                    if (l == THEN1__WD or THEN2__WD or THEN3__WD or comma_word) {
                        held_back_mode = 1; hb_wn = wn-1;
                    }
                    else {
                        for (m=0 : m<32 : m++) pattern2-->m = pattern-->m;
                        pcount2 = pcount;
                        etype = UPTO_PE;
                        break;
                    }
                }

                ! Now, we may need to revise the multiple object because of the single one
                ! we now know (but didn't when the list was drawn up).

                if (parameters >= 1 && results-->2 == 0) {
                    l = ReviseMulti(results-->3);
                    if (l ~= 0) { etype = l; results-->0 = action_to_be; break; }
                }
                if (parameters >= 2 && results-->3 == 0) {
                    l = ReviseMulti(results-->2);
                    if (l ~= 0) { etype = l; break; }
                }

                ! To trap the case of "take all" inferring only "yourself" when absolutely
                ! nothing else is in the vicinity...

                if (take_all_rule == 2 && results-->2 == actor) {
                    best_etype = NOTHING_PE;
                    jump GiveError;
                }

                #Ifdef DEBUG;
                if (parser_trace >= 1) print "[Line successfully parsed]^";
                #Endif; ! DEBUG

                ! The line has successfully matched the text.  Declare the input error-free...

                oops_from = 0;

                ! ...explain any inferences made (using the pattern)...
#Ifdef IMPRIMIR_DEDUCCIONES;                   ! (c) Alpha
                if (inferfrom ~= 0) {
                    print "("; PrintCommand(inferfrom); print ")^";
                }
#Endif;                                        ! (c) Alpha
                ! ...copy the action number, and the number of parameters...

                results-->0 = action_to_be;
                results-->1 = parameters;

                ! ...reverse first and second parameters if need be...

                if (action_reversed && parameters == 2) {
                    i = results-->2; results-->2 = results-->3;
                    results-->3 = i;
                    if (nsns == 2) {
                        i = special_number1; special_number1 = special_number2;
                        special_number2 = i;
                    }
                }

                ! ...and to reset "it"-style objects to the first of these parameters, if
                ! there is one (and it really is an object)...

                if (parameters > 0 && results-->2 >= 2)
                    PronounNotice(results-->2);

                ! ...and worry about the case where an object was allowed as a parameter
                ! even though the player wasn't holding it and should have been: in this
                ! event, keep the results for next time round, go into "not holding" mode,
                ! and for now tell the player what's happening and return a "take" request
                ! instead...

                if (not_holding ~= 0 && actor == player) {
                    action = ##Take;
                    i = RunRoutines(not_holding, before_implicit);
                    ! i = 0: Take the object, tell the player (default)
                    ! i = 1: Take the object, don't tell the player
                    ! i = 2: don't Take the object, continue
                    ! i = 3: don't Take the object, don't continue
                    if (i > 2) { best_etype = NOTHELD_PE; jump GiveError; }
                    if (i < 2) {        ! perform the implicit Take
                        if (i ~= 1)     ! and tell the player
                            L__M(##Miscellany, 26, not_holding);
                        notheld_mode = 1;
                        for (i=0 : i<8 : i++) kept_results-->i = results-->i;
                        results-->0 = ##Take;
                        results-->1 = 1;
                        results-->2 = not_holding;
                    }
                }

                ! (Notice that implicit takes are only generated for the player, and not
                ! for other actors.  This avoids entirely logical, but misleading, text
                ! being printed.)

                ! ...and return from the parser altogether, having successfully matched
                ! a line.

                if (held_back_mode == 1) {
                    wn=hb_wn;
                    jump LookForMore;
                }
                rtrue;

            } ! end of if(token ~= ENDIT_TOKEN) else
        } ! end of for(pcount++)

        ! The line has failed to match.
        ! We continue the outer "for" loop, trying the next line in the grammar.

        if (etype > best_etype) best_etype = etype;
        if (etype ~= ASKSCOPE_PE && etype > nextbest_etype) nextbest_etype = etype;

        ! ...unless the line was something like "take all" which failed because
        ! nothing matched the "all", in which case we stop and give an error now.

        if (take_all_rule == 2 && etype==NOTHING_PE) break;

    } ! end of for(line++)

    ! The grammar is exhausted: every line has failed to match.

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! H: Cheaply parse otherwise unrecognised conversation and return
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  .GiveError;

    etype = best_etype;

    ! Errors are handled differently depending on who was talking.
    ! If the command was addressed to somebody else (eg, "dwarf, sfgh") then
    ! it is taken as conversation which the parser has no business in disallowing.

    if (actor ~= player) {
        if (usual_grammar_after ~= 0) {
            verb_wordnum = usual_grammar_after;
            jump AlmostReParse;
        }
        wn = verb_wordnum;
        special_word = NextWord();
        if (special_word == comma_word) {
            special_word = NextWord();
            verb_wordnum++;
        }
        special_number = TryNumber(verb_wordnum);
        results-->0 = ##NotUnderstood;
        results-->1 = 2;
        results-->2 = 1; special_number1 = special_word;
        results-->3 = actor;
        consult_from = verb_wordnum; consult_words = num_words-consult_from+1;
        rtrue;
    }

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! I: Print best possible error message
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ! If the player was the actor (eg, in "take dfghh") the error must be printed,
    ! and fresh input called for.  In three cases the oops word must be jiggled.

    if (ParserError(etype) ~= 0) jump ReType;
    pronoun_word = pronoun__word; pronoun_obj = pronoun__obj;

    if (etype == STUCK_PE) {    L__M(##Miscellany, 27); oops_from = 1; }
    if (etype == UPTO_PE) {     L__M(##Miscellany, 28);
        for (m=0 : m<32 : m++) pattern-->m = pattern2-->m;
        pcount = pcount2; PrintCommand(0); L__M(##Miscellany, 56);
    }
    if (etype == NUMBER_PE)     L__M(##Miscellany, 29);
    if (etype == CANTSEE_PE) {  L__M(##Miscellany, 30); oops_from=saved_oops; }
    if (etype == TOOLIT_PE)     L__M(##Miscellany, 31);
    if (etype == NOTHELD_PE) {  L__M(##Miscellany, 32); oops_from=saved_oops; }
    if (etype == MULTI_PE)      L__M(##Miscellany, 33);
    if (etype == MMULTI_PE)     L__M(##Miscellany, 34);
    if (etype == VAGUE_PE)      L__M(##Miscellany, 35);
    if (etype == EXCEPT_PE)     L__M(##Miscellany, 36);
    if (etype == ANIMA_PE)      L__M(##Miscellany, 37);
    if (etype == VERB_PE)       L__M(##Miscellany, 38);
    if (etype == SCENERY_PE)    L__M(##Miscellany, 39);
    if (etype == ITGONE_PE) {
        if (pronoun_obj == NULL)
                                L__M(##Miscellany, 35);
        else                    L__M(##Miscellany, 40);
    }
    if (etype == JUNKAFTER_PE)  L__M(##Miscellany, 41);
    if (etype == TOOFEW_PE)     L__M(##Miscellany, 42, multi_had);
    if (etype == NOTHING_PE) {
        if (results-->0 == ##Remove && results-->3 ofclass Object) {
            noun = results-->3; ! ensure valid for messages
            if (noun has animate) L__M(##Take, 6, noun);
            else if (noun hasnt container or supporter) L__M(##Insert, 2, noun);
            else if (noun has container && noun hasnt open) L__M(##Take, 9, noun);
            else if (children(noun)==0) L__M(##Search, 6, noun);
            else results-->0 = 0;
            }
        if (results-->0 ~= ##Remove) {
            if (multi_wanted==100)  L__M(##Miscellany, 43);
            else                    L__M(##Miscellany, 44);
        }
    }
    if (etype == ASKSCOPE_PE) {
        scope_stage = 3;
        if (indirect(scope_error) == -1) {
            best_etype = nextbest_etype;
            jump GiveError;
        }
    }

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! J: Retry the whole lot
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ! And go (almost) right back to square one...

    jump ReType;

    ! ...being careful not to go all the way back, to avoid infinite repetition
    ! of a deferred command causing an error.


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! K: Last thing: check for "then" and further instructions(s), return.
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ! At this point, the return value is all prepared, and we are only looking
    ! to see if there is a "then" followed by subsequent instruction(s).

  .LookForMore;

    if (wn > num_words) rtrue;

    i = NextWord();
    if (i == THEN1__WD or THEN2__WD or THEN3__WD or comma_word) {
        if (wn > num_words) {
           held_back_mode = false;
           return;
        }
        i = WordAddress(verb_wordnum);
        j = WordAddress(wn);
        for (: i<j : i++) i->0 = ' ';
        i = NextWord();
        if (i == AGAIN1__WD or AGAIN2__WD or AGAIN3__WD) {
            ! Delete the words "then again" from the again buffer,
            ! in which we have just realised that it must occur:
            ! prevents an infinite loop on "i. again"

            i = WordAddress(wn-2)-buffer;
            if (wn > num_words) j = INPUT_BUFFER_LEN-1;
            else j = WordAddress(wn)-buffer;
            for (: i<j : i++) buffer3->i = ' ';
        }
        Tokenise__(buffer,parse);
        held_back_mode = true;
        return;
    }
    best_etype = UPTO_PE;
    jump GiveError;

]; ! end of Parser__parse

! ====================================================
! Usamos TestScope en lugar de ScopeCeiling
!
#ifdef INFSPR_adv;Message "   Incluyendo reemplazo ReviseMulti"; #endif;
[ ReviseMulti second_p  i low;
    #Ifdef DEBUG;
    if (parser_trace >= 4) print "   Revising multiple object list of size ", multiple_object-->0,
      " with 2nd ", (name) second_p, "^";
    #Endif; ! DEBUG

    if (multi_context == MULTIEXCEPT_TOKEN or MULTIINSIDE_TOKEN) {
        for (i=1,low=0 : i<=multiple_object-->0 : i++) {
            if ( (multi_context==MULTIEXCEPT_TOKEN && multiple_object-->i ~= second_p) ||
                 (multi_context==MULTIINSIDE_TOKEN && multiple_object-->i in second_p)) {
                low++;
                multiple_object-->low = multiple_object-->i;
            }
        }
        multiple_object-->0 = low;
    }

    if (multi_context == MULTI_TOKEN && action_to_be == ##Take) {
        for (i=1,low=0 : i<=multiple_object-->0 : i++)
            if (TestScope(multiple_object-->i, actor)) low++;
        #Ifdef DEBUG;
        if (parser_trace >= 4) print "   Token 2 plural case: number with actor ", low, "^";
        #Endif; ! DEBUG
        if (take_all_rule == 2 || low > 0) {
            for (i=1,low=0 : i<=multiple_object-->0 : i++) {
                if (TestScope(multiple_object-->i, actor)) {
                    low++;
                    multiple_object-->low = multiple_object-->i;
                }
            }
            multiple_object-->0 = low;
        }
    }

    i = multiple_object-->0;
    #Ifdef DEBUG;
    if (parser_trace >= 4) print "   Done: new size ", i, "^";
    #Endif; ! DEBUG
    if (i == 0) return NOTHING_PE;
    return 0;
];

! ====================================================
! Usamos TestScope en lugar de ScopeCeiling
!
#ifdef INFSPR_adv;Message "   Incluyendo reemplazo ChangePlayer"; #endif;
[ ChangePlayer obj flag i;
! if (obj.&number == 0) return RunTimeError(7,obj);
  if (actor == player) actor=obj;
  give player ~transparent ~concealed;
  i = obj; while (parent(i) ~= 0) {
      if (i has animate) give i transparent;
      i = parent(i);
  }
  if (player == selfobj) player.short_name = FORMER__TX;

  player = obj;

  if (player provides persona) AsignarPersona(player.persona); ! (c) Alpha

  if (player == selfobj) player.short_name = NULL;
  give player transparent concealed animate proper;
  i = player; while (parent(i) ~= 0) i = parent(i);
  location = i; real_location = location;
  if (parent(player) == 0) return RunTimeError(10);
  MoveFloatingObjects();
  lightflag = OffersLight(parent(player));
  if (lightflag == 0) location = thedark;
  print_player_flag = flag;
];


! ------------------------------------
! Verlib Replace Section
! ------------------------------------

! Nada por ahora

! Fin del archivo INFSPR
