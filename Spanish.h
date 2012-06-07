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
!                Spanish. Language Spanish File."
!================================================================
! Spanish.h
! INFSP 0.9 libreria espa�ola para inform, basada en informATE de Zak
!                         .: by Sarganar :.
!
! Definici�n de todo lo relacionado con el idioma Espa�ol (art�culos,
! nombres, pre-parsing, mensajes estandares, etc.)
!
!  InformATE es (c) de Zak
!  Re-edicion INFSP by Sarganar. Nuevas mejoras by colaboradores varios.
!  caad.es/informate/infsp/
! ----------------------------------------------------------------------------

System_file;
Message "^Incluyendo Spanish: Mensajes y rutinas de idioma [INFSP 0.9]";

#Ifndef VN_1630;
Message fatalerror "*** INFSP needs Inform v6.30 or later to work ***";
#Endif; ! VN_

! [INFSP] Replacemnet Rutines. See INFSPR.H -----------------------------------------------------------
Replace DictionaryLookup;
Replace PrefaceByArticle;
Replace Identical;
Replace TryGivenObject;
Replace Indefart;
Replace CInDefArt;
Replace BestGuess;
Replace NounDomain;
Replace Parser__parse;
Replace ReviseMulti;
Replace ChangePlayer;


! ---------------------------------------------------------------------------
!   Parte I.   Asuntos preliminares
! ---------------------------------------------------------------------------

Constant LanguageVersion = "Librer�a Espa�ola INFSP 0.9 [901.21], basada en InformATE!";

! Definici�n de caract�res "baratos". Esto permite que estas letras
! ocupen menos bits en la m�quina Z. No funciona si se intenta la
! compilaci�n por m�dulos (viejo m�todo)

#ifdef TARGET_ZCODE;
  Zcharacter '�';                    ! a con acento
  Zcharacter '�';                    ! e con acento
  Zcharacter '�';                    ! i con acento
  Zcharacter '�';                    ! o con acento
  Zcharacter '�';                    ! u con acento
  Zcharacter '�';                    ! n con virguilla
  Zcharacter '�';                    ! N con virguilla
  Zcharacter '�';                    ! u con dieresis
  Zcharacter '�';                    ! abrir interrogaci�n
  Zcharacter '�';                    ! abrir exclamaci�n
#endif;

!----------------------------------------------
! CONSTANTES Y CLASES ESPECIFICAS DE INFORMATE
!----------------------------------------------

!Constant G_MASCULINO = 0;
!Constant G_FEMENINO  = 1;
!Constant G_PLURAL    = 3;
Constant G_MASCULINO = $$001; ! 1 infsp new approach: para hacerlo bicompatible: I6/I7
Constant G_FEMENINO  = $$010; ! 2
Constant G_PLURAL    = $$100; ! 4

Class VerboIrregular;

!------------------------------------
! Globales espec�ficas de InformatE!
!------------------------------------

global PreguntaCualExactamente = 0;
! Esta variable se pone a 1 cuando se le pregunta al jugador �Cu�l
! exactamente...? esto indica que puede ser necesario re-traducir la
! respuesta que el jugador d�, ya que puede que simplemente el jugador
! cambie de idea y en lugar de responder a nuestra pregunta introduzca
! un comando completamente nuevo.

global PreguntaSiNo = 0;
! Esta variable debe ponerse a 1 manualmente si queremos que el verbo
! 'no' sea accesible como respuesta (y no como punto cardinal). La
! variable s�lo afecta a la siguiente respuesta del jugador y vuelve a
! hacerse cero autom�ticamente.

Global bandera_todo_vale; ! �y esta para qu� sirve?
Global quitacentos = 1;
Array  texto_impreso --> 52;

Global dialecto_sudamericano = 0;

Global FORMER__TX    = "tu antiguo ~yo~";
Global YOURSELF__TX  = "ti mismo";
Global CANTGO__TX    = "No puedes ir por ah�.";
Global IS__TX        = " ves";
Global ARE__TX       = " ves";
Global IS2__TX       = "ves ";
Global ARE2__TX      = "ves ";
Global YOU__TX       = "T�";
Global PARTICULA_TE  = "te";

!---------------------------------------------------------------------------
! Atributos y propiedades espec�ficas de InformatE!, tambien usados en INFSP
!---------------------------------------------------------------------------

Property additive adjectives;
Property gender 0;
Property imperativo alias name;
Property irrelevante alias name;
Property name_m alias name;
Property additive name_f;
Property additive name_fp;
Property additive name_mp;

#ifdef ADMITIR_COMANDO_SALIDAS;
  Property salidas;
#endif;

Attribute nombreusado; ! usado internamente cuando se matchea el objeto por el nombre usado



!================================================================
! Los puntos cardinales
!================================================================

Class   CompassDirection
  with  number 0, articles "El" "el" "",
        description [;
            if (location provides compass_look && location.compass_look(self)) rtrue;
            if (self.compass_look()) rtrue;
            L__M(##Look, 7, self);
        ],
        compass_look false,
  has   scenery;

Object Compass "br�jula" has concealed female ;

#Ifndef WITHOUT_DIRECTIONS;
  CompassDirection -> n_obj "norte"
    with
      name 'n//' 'norte',
      door_dir n_to;

  CompassDirection -> s_obj "sur"
    with
      name 's//' 'sur',
      door_dir s_to;

  CompassDirection -> e_obj "este"
    with
      name 'e//' 'este',
      door_dir e_to;

  CompassDirection -> w_obj "oeste"
    with
      name 'w//' 'o//' 'oeste',
      door_dir w_to;

  CompassDirection -> ne_obj "nordeste"
    with
      name 'ne' 'nordeste' 'noreste',
      door_dir ne_to;

  CompassDirection -> nw_obj "noroeste"
    with
      name 'no' 'nw' 'noroeste',
      door_dir nw_to;

  CompassDirection -> se_obj "sudeste"
    with
      name 'se' 'sureste' 'sudeste',
      door_dir se_to;

  CompassDirection -> sw_obj "sudoeste"
    with
      name 'so' 'sw' 'suroeste' 'sudoeste',
      door_dir sw_to;

  CompassDirection -> u_obj "arriba",
    with
      name 'u//' 'arriba' 'ar' 'r//' 'sube' 'techo' 'cielo',
      door_dir u_to,
    has
      proper;

  CompassDirection -> d_obj "abajo"
    with
      name 'd//' 'abajo' 'ab' 'b//' 'baja' 'piso' 'suelo',
      door_dir d_to,
    has
      proper;
#endif; ! WITHOUT_DIRECTIONS

CompassDirection -> in_obj "interior"
  with
    door_dir in_to,
    name 'en' 'dentro' 'adentro';

CompassDirection -> out_obj "exterior"
  with
    door_dir out_to,
    name 'fuera' 'afuera';

! ---------------------------------------------------------------------------
!   Parte II.   Vocabulario
! ---------------------------------------------------------------------------
Constant AGAIN1__WD = 'repetir';
Constant AGAIN2__WD = 're';
Constant AGAIN3__WD = 'g//';
Constant OOPS1__WD    = 'oops';
Constant OOPS2__WD    = 'epa';
Constant OOPS3__WD    = 'eepa';
Constant UNDO1__WD  = 'undo';
Constant UNDO2__WD  = 'deshacer';
Constant UNDO3__WD  = 'anular';

Constant ALL1__WD    = 'todos';
Constant ALL2__WD    = 'todas';
Constant ALL3__WD    = 'todo';
Constant ALL4__WD    = 'ambos';
Constant ALL5__WD    = 'ambas';
Constant AND1__WD       = 'y//';
Constant AND2__WD       = 'y//';
Constant AND3__WD       = 'e//';
Constant BUT1__WD   = 'excepto';
Constant BUT2__WD   = 'menos';
Constant BUT3__WD   = 'salvo';
Constant ME1__WD      = '-me';
Constant ME2__WD      = '-te';
Constant ME3__WD      = '-se';
Constant OF1__WD      = '.de';       ! no usados nunca, pues provocan
Constant OF2__WD      = '.de';       ! interferencia con la preposici�n "de"
Constant OF3__WD      = '.de';       ! usada por algunos verbos, como en
Constant OF4__WD      = '.de';       ! "saca objeto de la caja"
Constant OTHER1__WD    = 'otro';
Constant OTHER2__WD    = 'otra';
Constant OTHER3__WD    = 'otro';
Constant THEN1__WD = 'ydespues';
Constant DESPUES11_WD = 'despues';
Constant THEN2__WD = 'yluego';
Constant DESPUES21_WD = 'luego';
Constant THEN3__WD = 'yentonces';
Constant DESPUES31_WD = 'entonces';

Constant NO1__WD      = 'n//';
Constant NO2__WD      = 'no';
Constant NO3__WD      = 'no';
Constant YES1__WD      = 's//';
Constant YES2__WD      = 'si';
Constant YES3__WD      = 's�';

Constant AMUSING__WD = 'curiosidades';
Constant FULLSCORE1__WD  = 'puntuacion';
Constant FULLSCORE2__WD  = 'punt';
Constant QUIT1__WD    = 'q//';
Constant QUIT2__WD    = 'terminar';
Constant RESTART__WD    = 'reiniciar';
Constant RESTORE__WD    = 'recuperar';

Array LanguagePronouns table

!   palabra  GNAs que pueden                conectado
!            seguirle:                      a:
!              a     i
!              s  p  s  p
!              mfnmfnmfnmfn

    '-lo'    $$101000100001                    NULL
    '-los'   $$000101000101                    NULL
    '-la'    $$010000010000                    NULL
    '-las'   $$000010000010                    NULL
    '-le'    $$110000110000                    NULL
    '-les'   $$000110000110                    NULL
    '�l'     $$100000100000                    NULL
    'ella'   $$010000010000                    NULL
    'ellos'  $$000100000100                    NULL
    'ellas'  $$000010000010                    NULL;

Array LanguageDescriptors table

    !palabra  GNAs que pueden     tipo de   conectado
    !              seguirles:  descriptor:       con:
    !          a     i
    !          s  p  s  p
    !          mfnmfnmfnmfn                 
    
    'mi'     $$110110110110    POSSESS_PK      0
    'este'   $$100000100000    POSSESS_PK      0
    'estos'  $$000100000100    POSSESS_PK      0
    'esta'   $$010000010000    POSSESS_PK      0
    'estas'  $$000010000010    POSSESS_PK      0
    'ese'    $$100000100000    POSSESS_PK      1
    'esa'    $$010000010000    POSSESS_PK      1
    'esos'   $$000100000100    POSSESS_PK      1
    'esas'   $$000010000010    POSSESS_PK      1
    'su'     $$110000110000    POSSESS_PK      '-le'
    'sus'    $$000110000110    POSSESS_PK      '-le'
    'el'     $$100000100000    DEFART_PK       NULL
    'la'     $$010000010000    DEFART_PK       NULL
    'los'    $$000100000100    DEFART_PK       NULL
    'las'    $$000010000010    DEFART_PK       NULL
    'un'     $$100000100000    INDEFART_PK     NULL
    'una'    $$010000010000    INDEFART_PK     NULL
    'unos'   $$000100000100    INDEFART_PK     NULL
    'unas'   $$000010000010    INDEFART_PK     NULL
    'alg�n'  $$100000100000    INDEFART_PK     NULL
   'algunos' $$000100000100    INDEFART_PK     NULL
   'algunas' $$000010000010    INDEFART_PK     NULL
 'encendido' $$100000100000    light             NULL
 'encendida' $$010000010000    light             NULL
'encendidos' $$000100000100    light             NULL
'encendidas' $$000010000010    light             NULL
   'apagado' $$100000100000    (-light)          NULL
   'apagada' $$010000010000    (-light)          NULL
  'apagados' $$000100000100    (-light)          NULL
  'apagadas' $$000010000010    (-light)          NULL;

Array LanguageNumbers table
    'un' 1 'uno' 1 'una' 1 'dos' 2 'tres' 3 'cuatro' 4 'cinco' 5
    'seis' 6 'siete' 7 'ocho' 8 'nueve' 9 'diez' 10
    'once' 11 'doce' 12 'trece' 13 'catorce' 14 'quince' 15
    'diecis�is' 16 'diecisiete' 17 'dieciocho' 18 'diecinueve' 19
    'veinte' 20;

! ---------------------------------------------------------------------------
!   Parte III.   Traducci�n
! ---------------------------------------------------------------------------

!------------------------------------
! Funciones de ayuda a la depuraci�n
!------------------------------------

[ ImprimeBuffer b desde l i;
    for (i=desde:i<desde+l:i++)
    print (char) b->i;
];

[ ImprimeTodoElBuffer b;
#ifdef TARGET_ZCODE;
   ImprimeBuffer(b, 2, b->1);
#ifnot; !TARGET_GLULX
   ImprimeBuffer(b, WORDSIZE, b-->0);
#endif;
];

#ifdef TARGET_GLULX; ! TARGET_GLULX
[ PasarAMinuscula buffer   ! [INFSP] Necesario en EspanolAInformes (en modo Glulx), para q el buffer
        i;                 !         sea reconocido (al usar infinitivo en mayuscula (EXAMINAR) o verbos pronominales (EXAMINALA)
    for (i=WORDSIZE:i<(WORDSIZE+(buffer-->0)):i++)
        buffer->i = LowerCase (buffer->i);
    return buffer;
];
#endif; !TARGET_

!----------------------------------------------------------------
! [010515] Rutina nueva, que recibe una palabra de diccionario y un
! lugar donde buscar, y retorna "true" si alguno de los objetos de ese
! lugar tiene esa palabra en su campo nombre, name_f...
!
! Es usada por la rutina ArreglarLosY, pues se mira si la palabra que
! viene detr�s de la Y es un nombre, y si lo es, si es aplicable a alg�n
! objeto cercano. Observar que fallar� en los objetos que proporcionen
! su propio parse_name. 
!----------------------------------------------------------------
[ EncajeSimplon w donde i;
  objectloop(i in donde) {
    if (WordInProperty(w, i, name)) rtrue;
    if (WordInProperty(w, i, name_f)) rtrue;
    if (WordInProperty(w, i, name_mp)) rtrue;
    if (WordInProperty(w, i, name_fp)) rtrue;
    if (WordInProperty(w, i, adjectives)) rtrue;
    if (children(i) && EncajeSimplon(w, i)) rtrue;
  }
  rfalse;
];


! ----------------------------------------------------------------------------
!  Las siguientes pueden ser �tiles para los programadores de los
!  ficheros de definici�n de lenguaje
! ----------------------------------------------------------------------------

! InsertarLetraBuffer ya es soportada por la funcion de f�brica LTI_Insert

[ BorrarBuffer b p i; ! infsp TODO : esta funcion no se usa, la borramos?
    ! Rellena con espacios el buffer
    for (i=2:i<INPUT_BUFFER_LEN:i++) b->i=' ';
    Tokenise__(b, p);
];



!----------------------------------------------------------------
! QuitarAcentos
!
! La siguiente funci�n prueba a quitar los acentos de las palabras que
! no hayan podido ser interpretadas. De este modo, el jugador puede usar
! o no los acentos. Para ello el juego debe definir las palabras sin acentos
! por ejemplo:
! object sillon "sill�n confortable"
! with name 'sillon' 'confortable'
!
! Si el jugador pone: "mira sillon", la palabra encaja directamente y es
! comprendida. Si pone "mira sill�n", no es comprendida, de modo que se
! le quita el acento y se reintenta el parsing.
!
! Este caso no es especialmente importante, pues el programador
! siempre puede definir 'sillon' y 'sill�n', ambos como nombres para
! el objeto. Pero el asunto es m�s importante para el caso de los
! verbos, ya que si el jugador teclea "c�gelo" este es separado en
! "c�ge" y "-lo" y el verbo "c�ge" (con acento) no ser�a
! comprendido.
!
! La llamada a esta funci�n puede desactivarse con el comando de
! debugging "acentos s�" de modo que los acentos que teclee el jugador
! no ser�n eliminados (con prop�sitos de depuraci�n)

[ QuitarAcentos buf pars x i word at len;

    if (parser_trace >= 7) ! imprimir el buffer recibido
    {
          print "^[ QuitarAcentos:  ";
          print "Buffer recibido: |";
             ImprimeTodoElBuffer(buf);
          print "|^";
    } 
    tokenise__(buf, pars); 
    for (x=1:x<=tokenCount(pars):x++) ! para cada token
    {
  word = tokenDict (pars,x); !dictionary value of token 1,2,3...
  at = tokenPos (pars,x); ! position in buffer of token 1,2,3... 
  len = WordLength (x);! length in chars of token 1,2,3...

  if (word==0) ! no comprendida
       {
    if (parser_trace>=7)
    {
        print "    QuitarAcentos:NO COMPRENDIDO: |";
      ImprimeBuffer(buf, at, len);
        print "|^";
    }
    for (i=at:i<at+len:i++)
       switch(buf->i)
      {
      '�': buf->i='a';
      '�': buf->i='e';
      '�': buf->i='e';
      '�': buf->i='i';
      '�': buf->i='o';
      '�': buf->i='u';
      '�': buf->i='u';
      '�': buf->i='n'; 
    !             '?': buf->i=' ';
    !             '�': buf->i=' ';
    !             '!': buf->i=' ';
    !         '�': buf->i=' ' ;    
       }
        tokenise__(buf, pars); 
        }
    }

    if (parser_trace>=7){
        print "    Buffer sin acentos: |";
        ImprimeTodoElBuffer(buf);
        print "|^";
    }
];

! funciones de soporte para gestionar parse y buffer
!  From InformFAQ - Roger Firth

[ tokenDict pars w; ! dictionary value of token 1,2,3...
#Ifdef TARGET_ZCODE;
  return pars-->(2*w - 1);
#Ifnot; ! TARGET_GLULX
  return pars-->(3*w - 2);
#Endif; ! TARGET_
];
[ tokenLen pars w; ! length in chars of token 1,2,3...
#Ifdef TARGET_ZCODE;
  return pars->(4*w);
#Ifnot; ! TARGET_GLULX
  return pars-->(3*w - 1);
#Endif; ! TARGET_
];
[ tokenPos pars w; ! position in buffer of token 1,2,3...
#Ifdef TARGET_ZCODE;
  return pars->(4*w + 1);
#Ifnot; ! TARGET_GLULX
  return pars-->(3*w);
#Endif; ! TARGET_
];

[ tokenCount pars;
#Ifdef TARGET_ZCODE;
  return pars->1;
#Ifnot; ! TARGET_GLULX
  return pars-->0;
#Endif; ! TARGET_
];


!-----------------------------------------------------
! LanguageToInformese
!
! Esta parte es interesante. Debe separar los sufijos pronominales
! como en "cogela" "mirale", claro que no basta con ver si las dos
! �ltimas letras del verbo son "la" "le", etc, pues el verbo podr�a
! terminar de forma "natural" en esas letras (como en "habla"). As�
! que hay que separar el sufijo s�lo si el verbo no ha sido hallado en
! el diccionario.
!
[ LanguageToInformese ;

    EspanolAInformes(buffer, parse);
];

! La funci�n que verdaderamente hace la traducci�n es la siguiente. Ha
! sido separada de LanguageToInformese porque es llamada tambi�n desde
! LanguageIsVerb, pero recibe un buffer diferente para procesar.

[ EspanolAInformes buf pars x i word at len romper_en siguiente retokenise at2;

!    print "   EspanolAInformes called^"; ! infsp debug
    

#ifdef TARGET_GLULX; ! TARGET_GLULX
    buf = PasarAMinuscula (buf); ![INFSP] Necesario para q no cazque al procesar infinitivos
#endif;

    tokenise__( buf, pars);

    retokenise=0;
    if (parser_trace>=7) !imprimir el buf recibido
    {
          print "^[ EspanolAInformes:  ";
          print "Buffer recibido: |";
    ImprimeTodoElBuffer(buf);
          print "|^  ";
    } 
    if (quitacentos) QuitarAcentos(buf, pars);

    for (x=1:x<=tokenCount(pars):x++) ! para cada token
    {
  word = tokenDict (pars,x); !dictionary value of token 1,2,3...
  at = tokenPos (pars,x); ! position in buffer of token 1,2,3... 
  len = tokenLen (pars,x);! length in chars of token 1,2,3...

  if (parser_trace>=7){
          print "Palabra ",x," : ";
          if (word==0)
            print "<no comprendida>";
          else 
            print (address) word," ^";
          print " ^";
        }        

  if (dialecto_sudamericano && (word == 'coge'))
  {
    ! Un poco bestia, pero funciona. En caso de dialecto sudamericano,
    ! cambiar un par de letras convierte "coge" en "jode"
    buf->(at)     = 'j';
    buf->(at + 2) = 'd';
    retokenise    = 1;
    if (dialecto_sudamericano == 1)  ! [071030_git]
    {
      print "[Este juego utiliza por defecto el dialecto sudamericano,
        por lo que aqu� COGER es entendido como una palabra
        malsonante. Por favor, utiliza TOMAR en su lugar, o bien
        elige el dialecto castellano con DIALECTO CASTELLANO.]^";
      dialecto_sudamericano = 2;
    }
  }
    
    if (word=='del' or 'al')
    {
        buf->(at+len-1)=' ';    ! convertir "del" "al" en "de"
                                       ! "a" respectivamente
        retokenise=1;
    }
    

    ! Si el jugador ha escrito "no", y previamente el programador
    ! hab�a puesto a 1 el flag "PreguntaSiNo" (que indica que
    ! acaba de hacerse una pregunta que podr�a admitir NO como
    ! respuesta), en ese caso cambiamos "no" por "xo". La
    ! gram�tica define el verbo "xo" para generar la acci�n NO
    ! (negar), y as� lo distinguimos de la acci�n "Ir noroeste"
    if ((word=='no')&&(PreguntaSiNo==1))
    {
        buf->(at+len-1)='x';
        PreguntaSiNo=0;
        retokenise=1;
    }
        
        ! "pegar" las expresiones "y luego" "y despues" "y entonces"
        ! para que queden "yluego" "ydespues" "yentonces"
        ! que es como las reconoce el parser
     if (word=='y//')
    {
       siguiente= tokenDict (pars,x+1); ! palabra siguiente

  if (siguiente==DESPUES11_WD or DESPUES21_WD or DESPUES31_WD)
       {
     at2 = tokenPos (pars,x+1); ! position in buffer of token 1,2,3... 
           for (i=0:i<tokenLen(pars,x+1):i++){
                   buf->(at2+i) = ' '; ! borrar palabra 'entonces' (pis�ndola con espacios)
     }
           buf->(at+len)='l'; ! agregar 'luego' inmediatamente despues de 'y', formando 'yluego'
           buf->(at+len+1)='u';
           buf->(at+len+2)='e';
           buf->(at+len+3)='g';
           buf->(at+len+4)='o';
           retokenise=1;
           if (parser_trace>=7) !imprimir el buf recibido
     {
    print "^[   YLUEGO:  ";
    print "Buffer recibido: |";
    ImprimeTodoElBuffer(buf);
    print "|^  ";
      } 
       }
    }

    if (word==0) ! no comprendida
    {
        if (parser_trace>=7)
        {
            print "NO COMPRENDIDO: |";
            ImprimeBuffer(buf, at, len);
            print "|^";
        }
        
        if (len > 2) !s�lo si tiene m�s de tres letras
        {
           romper_en=0;
           if ((buf->(at+len-1)=='s') &&
            (buf->(at+len-2)=='a' or 'o' or 'e') &&
            (buf->(at+len-3)=='l'))
            romper_en=3;
           if ((buf->(at+len-1)=='a' or 'o' or 'e') &&
            (buf->(at+len-2)=='l'))
            romper_en=2;
           if ((buf->(at+len-1)=='e') &&
            (buf->(at+len-2)=='m' or 't' or 's'))
            romper_en=2;  ! [001108] Antes pon�a == �ser� burro!
           if (romper_en) 
           {
            ! [990524]
            ! Antes de modificar el buffer, comprobemos si
            ! servir� para algo, es decir, si la palabra que
            ! queda una vez separado el sufijo existe en el
            ! diccionario y es un verbo
            i=DictionaryLookup(buf+at, len-romper_en);
            
              if (i) if ((i->#dict_par1)&1)
              {
                LTI_Insert(at+len-romper_en, '-', buf);
                LTI_Insert(at+len-romper_en, ' ', buf);
                tokenise__(buf,pars);
               }
              if (QuitandoRFinal(buf+at, len-romper_en)||
                  BuscarEntreVerbosIrregulares(buf+at, len-romper_en))
              {
                LTI_Insert(at+len-romper_en, '-', buf);
                LTI_Insert(at+len-romper_en, ' ', buf);
                tokenise__(buf,pars);
               }
             } ! if (romper_en
        } ! if (len > 2
    } ! if (word==0
    } ! for (x=0:

    if (retokenise==1) ! [INFSP] Agregado if, como en la seccion Glulx
       tokenise__(buf, pars);

    ArreglarLosY(buf, pars);
    
    ! Como paso final, quitamos las palabras que aparezcan dos veces
    ! seguidas. Esto puede ocurrir si el jugador teclea un comando
    ! incompleto, como "MIRA HACIA", y el parser le pregunta "�Hacia
    ! donde quieres mirar?" y el jugador responde "HACIA EL NORTE". En
    ! este caso se construir�a la orden concatenada: "MIRA HACIA HACIA
    ! EL NORTE".
    if (PreguntaCualExactamente==1){
        PreguntaCualExactamente=0;
        EliminarDuplicados(buf, pars);
    }

    ! Si la frase acaba en 'de' o 'del', quitamos esas dos part�culas.
    ! Si acaba en 'el', entendemos que se refiere a un pronombre, y lo
    ! sustituimos por '-lo':
    retokenise = 0;
    x = tokenCount(pars);
    word = tokenDict(pars, x);
    at   = tokenPos(pars, x);
    len  = tokenLen(pars, x);
    if (word == 'de') {
        buf->at = ' '; buf->(at + 1) = ' ';
        retokenise = 1;
    } else if (word == 'del') {
        buf->at = ' '; buf->(at + 1) = ' '; buf->(at + 2) = ' ';
        retokenise = 1;
    #ifdef TARGET_ZCODE;
    } else if (word == 'el' && buf->1 < buf->0) {
    #ifnot; ! TARGET_GLULX
    } else if (word == 'el' && buf-->0 < INPUT_BUFFER_LEN) {
    #endif; ! TARGET_
        buf->at = '-'; buf->(at + 1) = 'l'; buf->(at + 2) = 'o';
        #ifdef TARGET_ZCODE;
        (buf->1)++;
        #ifnot; ! TARGET_GLULX
        (buf-->0)++;
        #endif; ! TARGET_
        retokenise = 1;
    }

    ! Corrige el poblema siguiente:
    ! > SACA TODO
    ! �De d�nde quieres sacar esas cosas?
    ! > DEL ARMARIO
    ! [Aqu� la tokenizaci�n quedar�a: 'saca' 'todo' 'de' 'del' 'armario']
    ! Lo que hacemos es poner un espacio en la 'd' de 'del', para
    ! convertirlo en 'el', y as� la tokenizaci�n quedar�a:
    ! 'saca' 'todo' 'de' 'el' 'armario'
    for (x = 1: x <= tokenCount(pars): x++) {
        word = tokenDict(pars, x);
        at   = tokenPos(pars, x);
        len  = tokenLen(pars, x);
        if (word == 'de' && x < tokenCount(pars)) {
            siguiente = tokenDict(pars, x + 1);
            if (siguiente == 'del') {
                at = tokenPos(pars, x + 1);
                buf->at = ' '; ! Ponemos un ' ' encima de la 'd' de 'del'
                retokenise = 1;
                break;
            }
        }
    }

    ! Corrige el problema del uso de reflexivos, tipo:
    ! > EXAMINATE A TI MISMO
    ! > EXAMINATE A TI
    for (x = 1: x <= tokenCount(pars): x++) {
        word = tokenDict(pars, x);
        at   = tokenPos(pars, x);
        len  = tokenLen(pars, x);
        if (word == ME1__WD or ME2__WD or ME3__WD && x < tokenCount(pars)) {
            siguiente = tokenDict(pars, x + 1);
            at = tokenPos(pars, x + 1);
            if (siguiente == 'a//' && (x + 1) < tokenCount(pars)) {
                siguiente = tokenDict(pars, x + 2);
                if (((word == ME1__WD && siguiente == 'mi') ||
                    (word == ME2__WD && siguiente == 'ti') ||
                    (word == ME3__WD && siguiente == 'si'))) {
                    if ((x + 2) < tokenCount(pars)) {
                        siguiente = tokenDict(pars, x + 3);
                        retokenise = 1;
                        if (siguiente == 'mismo' or 'misma') {
                            ! Hay que borrar tres palabras:
                            for (i = at: i < tokenPos(pars, x + 3) + tokenLen(pars, x + 3): i++) {
                                buf->i = ' ';
                            }
                        } else {
                            ! Hay que borrar dos palabras:
                            for (i = at: i < tokenPos(pars, x + 3) - 1: i++) {
                                buf->i = ' ';
                            }
                        }
                        break;
                    } else {
                        ! No hay m�s palabras; se queda en "-te a ti":
                        for (i = at: i < tokenPos(pars, x + 2) + tokenLen(pars, x + 2): i++) {
                            buf->i = ' ';
                        }
                        retokenise = 1;
                        break;
                    }
                }
            }
        }
    }

    if (parser_trace>=7){
       print "Buffer traducido a informese: ^|";
        ImprimeTodoElBuffer(buf);
       print "| (", pars->1," palabras) ^";
    }
];



[ ArreglarLosY buf pars
    word at len x i retoke;

    for (x=1:x<=tokenCount(pars):x++) ! para cada token
    {
   word = tokenDict (pars,x); !dictionary value of token 1,2,3...
        if (word=='y//')
  {
    i = tokenDict (pars,x+1); ! palabra siguiente
        ! [010515] arreglado bug. at y len no apuntaban a la
        ! palabra siguiente, sino a la propia 'y', por lo que no
        ! eran reconocidos los verbos en infinitivo
    at = tokenPos (pars,x+1); ! position in buffer of token siguiente 
    len = tokenLen (pars,x+1);! length in chars of token siguiente

        ! [010515] A�adido. Si la palabra es noun, se retorna
        ! (aunque pueda ser tambi�n un verbo) de este modo, en
        ! COGE VINO Y CAVA, si el juego tiene un objeto llamado
        ! "cava", la frase ser� parseada como tratando de coger
        ! dos objetos, llamados "vino" y "cava". Si el juego no
        ! define el sustantivo "cava", la frase ser� parseada como
        ! dos acciones en secuencia <<Coger vino>> y <<Excavar>>
    if (i && (i->#dict_par1)&128) {
      if (EncajeSimplon(i, location)) continue;
    }
    if (i && ((i->#dict_par1)&1)||QuitandoRFinal(buf+at,len)||
      BuscarEntreVerbosIrregulares(buf+at, len))
    {
      at = tokenPos (pars,x); ! position in buffer of token siguiente 
      buf->(at)='.';
      retoke=true;
    }
  }
    }
    if (retoke) tokenise__(buf, pars);
];

[ EliminarDuplicados buf pars
    word at len x i siguiente retokenise;

!    tokenise__(buf, pars);
    if (parser_trace>=8) !imprimir el buffer recibido
    {
          print "^[ EliminarDuplicados:  ";
          print "Buffer recibido: |";
              ImprimeTodoElBuffer(buf);
          print "| (", pars->1, " palabras)^";
    } 
    for (x=1:x<=tokenCount(pars):x++) ! para cada token
    {
  word = tokenDict (pars,x); !dictionary value of token 1,2,3...
  siguiente = tokenDict (pars,x+1); !dictionary value of token 1,2,3...
  at = tokenPos (pars,x); ! position in buffer of token 1,2,3... 
  len = tokenLen (pars,x);! length in chars of token 1,2,3...
    if ((word==siguiente) && (word~=0))
    {
        if (parser_trace>=8) print "Encontrados duplicados!:",
    (address) word, "=", (address) siguiente, ".^";
        for (i=at:i<at+len:i++) buf->i=' ';
        retokenise=1;
    }
    }
    if (retokenise) 
    tokenise__(buf, pars);

    if (parser_trace>=8) !imprimir el buffer recibido
    {
          print "Buffer resultante: |";
        ImprimeTodoElBuffer(buf);
          print "| (", pars->1, " palabras)^";
    } 
];


! La siguiente funci�n debe devolver si el verbo recibido es
! reconocido o no. Se recibe el buffer completo y "pos" indica qu�
! palabra dentro del buffer "pars" es el supuesto verbo.
! Hay que devolver la direcci�n de diccionario del verbo en cuesti�n.
!
! Lo usamos para admitir infinitivos. El m�todo es:
!   Si la palabra corresponde a una palabra de diccionario, no hacemos
!   nada (retornamos esa palabra, sin molestarnos en comprobar que sea
!   un verbo. Ya lo har� el parser).
!
!   Si la palabra no est� en el diccionario, miramos si termina en
!   'r', y de ser as� se la quitamos y re-tokenizamos. Si de este modo
!   se obtiene una palabra en el diccionario, retornamos su valor (sin
!   comprobar que sea un verbo, ya lo har� el parser).
!
!   Si tampoco de este modo se reconoce, comparamos la palabra dada
!   con todos los verbos irregulares conocidos. Si en uno de ellos
!   encontramos coincidencia, retornamos el primer elemento de la
!   propiedad "imperativo" del verbo en cuesti�n.
!
!   En el caso de la desambiguacion, puede darse que el jugador responda
!   con un verbo nuevo, pero con sufijo pronominal la/le/lo/me/te/se (ej. examinala)
!   Por ello se hace una llamada a EspanolAInformes

[ LanguageIsVerb buf pars pos
    i word at len;
    
    if (parser_trace>=7) !imprimir el buffer recibido
    {
  print "^[ LanguageIsVerb:  ";
  print "Buffer recibido: |";
    ImprimeTodoElBuffer(buf);
  print "|, pos=", pos, "^  ";
    } 

    if (PreguntaCualExactamente) ! caso de desambiguaci�n
  EspanolAInformes(buf, pars); ! revisar la existencia de sufijos pronominales la/le/lo/me/te/se

    word = tokenDict (pars,pos); !dictionary value of token 1,2,3...
    at = tokenPos (pars,pos); ! position in buffer of token 1,2,3... 
    len = tokenLen (pars,pos);! length in chars of token 1,2,3...

    ! Si la palabra se reconoce, devolverla sin m�s
     if (word~=0 && word->#dict_par1 & 1 ~= 0)
    {
  if (parser_trace>=7) print " la palabra ~", (address)
    word, "~ est� en el diccionario.^";
  return    (word);
    }

   
    ! Si no, probar a ver si se reconoce sin la 'r' final
    i=QuitandoRFinal(buf + at,len);

    ! Si no la ha reconocido, intentamos ver si es un verbo irregular
    if (i~=0)
    {
  if (parser_trace>=7) print "^ Verbo reconocido
    al quitarle la 'r' final.^";
    }
    else 
    {
  if (parser_trace>=7) print "^ Verbo no reconocido sin la
    'r'. ^Probando irregulares...";

  i=BuscarEntreVerbosIrregulares(buf + at,len);
  if (parser_trace>=7) 
  {
    if (i==0) print "No reconocido como irregular.^";
    else print "      Reconocido como el verbo ", (address) i, "^";
  }
    }

    if (parser_trace>=7) 
        print " ^                   -- end LanguageIsVerb ]^";
    return i;    
];

!#ifdef ADMITIR_INFINITIVOS;
[ QuitandoRFinal texto longitud
    i;

    if (parser_trace>=9) {
    print "^[ QuitandoRFinal: buffer recibido |";
    ImprimeBuffer(texto, 0, longitud);
    print "|^";
    }
    if (texto->(longitud-1)~='r')
    {
    if (parser_trace>=9) print "No termina en 'r'^";
    return 0;
    }
    
    i=DictionaryLookup(texto, longitud-1);
!    if ((dialecto_sudamericano==1)&&(i=='coge')) return 'jode';
    if (dialecto_sudamericano && (i == 'coge'))
    {
      if (dialecto_sudamericano == 1)  ! [071030] git
      {
        print "[Este juego utiliza por defecto el dialecto sudamericano,
                por lo que aqu� COGER es entendido como una palabra
                malsonante. Por favor, utiliza TOMAR en su lugar, o bien
                elige el dialecto castellano con DIALECTO CASTELLANO.]^";
        dialecto_sudamericano = 2;
      }
      return 'jode';
    }
    if ((i->#dict_par1)&1) return i;
    else 
    {
    if (parser_trace>=9) print "Est� en el diccionario, pero no es
        verbo.^";
    return 0;
    }
];

[ BuscarEntreVerbosIrregulares texto longitud
    verbo i k;

    objectloop (verbo ofclass VerboIrregular)
    {
    if (parser_trace>=8) print "^[Bucando irregulares. Probando
        con ~", (name) verbo, "~.-->";
#ifdef TARGET_ZCODE;
    texto_impreso-->0 = 50;
        @output_stream 3 texto_impreso;
        print (name) verbo;
        @output_stream -3;
    k=texto_impreso-->0;
#endif;
#ifdef TARGET_GLULX;
        k=PrintAnyToArray(texto_impreso+WORDSIZE, 46, verbo);
#endif; !TARGET_
    
        !Si sus longitudes no coinciden no es necesario seguir comparando
        if (k~=longitud) jump NoCoincide;
    
        !Si sus longitudes coinciden, miramos letra a letra
        for (i=0:i<k:i++)
            if ((texto->i)~=(texto_impreso->(i+WORDSIZE)))
        jump NoCoincide; ! si una sola letra no coincide,
    ! Si ha pasado los test anteriores, hemos hallado una
    ! coincidencia
    if (parser_trace>=8) print "�coincide!^";
    return (verbo.&imperativo-->0);

    .NoCoincide;
    if (parser_trace>=8) print "no coincide^";
    }
    return 0;
];
!#endif; !ADMITIR_INFINITIVOS


! ---------------------------------------------------------------------------
!   Parte IV.   Imprimir
! ---------------------------------------------------------------------------


! Definimos una funci�n para imprimir el verbo coger. Esto hace m�s f�cil
! la adaptaci�n de los juegos a los dialectos sudamericanos, en los
! que debe usarse "tomar" en lugar de "coger"
! 
! La variable global que elige el dialecto se fija en tiempo de
! ejecuci�n con el meta-verbo "dialecto". Tambi�n puede fijarse en
! Initialize para que su valor por defecto sea 1.
!
[ coge sufijo;
    if (dialecto_sudamericano)
      print "toma";
    else print "coge";
    if (sufijo ~= 0)
       print (string) sufijo;
];

! La siguiente es la versi�n con la primera letra may�scula

[ MCoge sufijo;
    if (dialecto_sudamericano)
    print "Toma";
    else print "Coge";
    if (sufijo ~= 0)
       print (string) sufijo;
];

! Y finalmente una versi�n con todo may�sculas

[ MMCoge sufijo;
    if (dialecto_sudamericano)
    print "TOMA";
    else print "COGE";
    if (sufijo ~= 0)
       print (string) sufijo;
];
    


Constant LanguageAnimateGender   = male;
Constant LanguageInanimateGender = male;

Constant LanguageContractionForms = 1;     ! Para el espa�ol
! No se produce contracci�n del art�culo. S�lo de las preposiciones
! como en "del" y "al", pero eso no est� contemplado por el manual
! del traductor, por lo que es necesario otro tipo de truco

[ LanguageContraction ;
    return 0;
];

Array LanguageArticles -->
    ! Forma de contracci�n 0:
    ! Cdef   Def  Indef  
    "El " "el " "un "    
    "La " "la " "una "   
    "Los " "los " "unos "  
    "Las " "las " "unas ";

                   !             a           i
                   !             s     p     s     p
                   !             m f n m f n m f n m f n                 
Array LanguageGNAsToArticles --> 0 1 0 2 3 2 0 1 0 2 3 2;

[ LanguageDirection d; ! [INFSP] Se agregaron los articulos, para una correcta impresion.
    switch(d)          !         Usado en PrintCommand y en Salidas.
 {   n_to: print "al norte";
     s_to: print "al sur";
     e_to: print "al este";
     w_to: print "al oeste";
     ne_to: print "al nordeste";
     nw_to: print "al noroeste";
     se_to: print "al sudeste";
     sw_to: print "al suroeste";
     u_to: print "arriba";
     d_to: print "abajo";
     in_to: print "dentro";
     out_to: print "fuera";
     default: return RunTimeError(9,d);
    }
];

[ LanguageNumber n venti m;
    if (n==0) { print "cero"; rfalse; }
    if (n<0) { print "menos "; n=-n; }
    if (n>=1000) { 
        if (n<2000) print "mil";
        else { LanguageNumber(n/1000,1); print " mil";} 
        n=n%1000; 
        if (n~=0) print " ";
        }
    if (n>=100)  { 
     CientosEspanol(n/100); 
         if (n%100~=0) print " ";
     n=n%100; 
    }
    if (n==0) rfalse;

    if (n<10) {
    DigitoEspanol(n); rfalse; }
    if (n>=20)
    {   m=n/10;
          if (n%10~=0) {
              if (m==2) print "veinti";
          } else {
              if (m==2) print "veinte";
          }
        if (m==3) print "treinta";
          if (m==4) print "cuarenta";
          if (m==5) print "cincuenta";
          if (m==6) print "sesenta";
          if (m==7) print "setenta";
          if (m==8) print "ochenta";
          if (m==9) print "noventa";
          if (n%10==0) rfalse;
          if (m>2) print " y "; 
          DigitoEspanol(n%10, (m==2)&&(venti==0)); rfalse;
    }
    if (n==10) { print "diez"; rfalse; }
    if (n==11) { print "once"; rfalse; }
    if (n==12) { print "doce"; rfalse; }
    if (n==13) { print "trece"; rfalse; }
    if (n==14) { print "catorce"; rfalse; }
    if (n==15) { print "quince"; rfalse; }
    if (n==16) { print "diecis�is"; rfalse; }
    if (n==17) { print "diecisiete"; rfalse; }
    if (n==18) { print "dieciocho"; rfalse; }
    print "diecinueve";
];

[ DigitoEspanol n venti;
    if (n==1) { print "un"; rfalse; }
    if (n==2) { 
         if (venti) {print "d�s";} else {print "dos"; };
         rfalse; }
    if (n==3) { 
         if (venti) {print "tr�s";} else {print "tres"; };
         rfalse; }
    if (n==4) { print "cuatro"; rfalse; }
    if (n==5) { print "cinco"; rfalse; }
    if (n==6) { 
         if (venti) {print "s�is";} else {print "seis"; };
         rfalse; }
    if (n==7) { print "siete"; rfalse; }
    if (n==8) { print "ocho"; rfalse; }
    if (n==9) { print "nueve"; rfalse; }
];

[ CientosEspanol n;

if (n==1) { print "ciento"; rfalse; }
if (n==2) { print "dos";}
if (n==3) { print "tres";}
if (n==4) { print "cuatro";}
if (n==5) { print "quinientos"; rfalse;}
if (n==6) { print "seis";}
if (n==7) { print "sete";}
if (n==8) { print "ocho";}
if (n==9) { print "nove";}
print "cientos"; rfalse;
];

[ LanguageTimeOfDay hours mins i;
   i=hours%12; if (i<10) print " ";
   if (i==0) i=12;
   print i, ":", mins/10, mins%10;
   if ((hours/12) > 0) print " pm"; else print " am";
];

[ LanguageVerb i aux;

    if (ImprimirIrregular(i)==0){
        !print "^   LanguageVerb: Verbo no es irregular.^"; ! infsp debug
      
      ! Tatar de matchear con la br�jula - 807.7 infsp
      objectloop (aux ofclass CompassDirection){
        if (WordInProperty(i, aux, name)) {
          print "ir hacia alg�n lado";
!          !LanguageDirection (aux.door_dir); ! the direction name as adverb
          rtrue; ! no avanzar m�s en la rutina
        }    
       }
       
       ! Un ultimo intento, con una rutina stub a proveer por el autor
       if (PrintVerb(i)==0){
         print (address) i; print "r";
        }
    }
     if (i=='mete') rtrue;
         
#ifdef TARGET_ZCODE;
    ! Comprobemos si era un verbo reflexivo (-te)
    texto_impreso-->0 = 50;
    @output_stream 3 texto_impreso;
    print (address) i;
    @output_stream -3;
    aux=2;
!    print "^   LanguageVerb: i : ",(address) i,"^"; ! infsp debug
    if (texto_impreso->(texto_impreso->1+1)=='e')
     if (texto_impreso->(texto_impreso->1) == 't' or 's' or 'm')
     {
!     print "^   LanguageVerb:Verbo reflexivo."; ! infsp debug
       if (actor ~= player) print "se";
        else print (string) PARTICULA_TE; ! "te"
     }
#endif;
#ifdef TARGET_GLULX;
    aux=PrintAnyToArray(texto_impreso, 50, i);
    if ((texto_impreso->(aux-1)=='e')&&
        (texto_impreso->(aux-2) == 't' or 's' or 'm')) 
    {
       if (actor ~= player) print "se";
        else print (string) PARTICULA_TE;
    }
#endif;
    rtrue;

];


[ ImprimirIrregular v i;
  ! Imprime los verbos irregulares (es decir, aquellos en los que no basta
  ! a�adir una "r" al imperativo). Por ejemplo, si el jugador teclea "friega"
  ! la respuesta por defecto ser�a "�Qu� quieres friegar?"
  ! Para evitar esto, el programador debe definir un objeto de la clase
  ! VerboIrregular, con nombre "fregar" y propiedad name = "friega"
  ! Esta rutina busca entre los verbos as� definidos, en el campo name
  ! y si encuentra una coincidencia imprime el nombre del objeto para
  ! el cual se hall�.

  ! Para mayor flexibilidad a�n, se permite que el programador incluya
  ! una rutina en la propiedad nombre_corto del verbo, por si el nombre
  ! del verbo a imprimir es variable seg�n el momento en que se llame.
  ! Si existe nombre_corto para el verbo en cuesti�n, se ejecuta dicha rutina
  ! de lo contrario se imprime el nombre de ese verbo sin m�s.

  ! Ver SpanishG.h para ejemplos de declaraci�n de verbos irregulares.

  objectloop (i ofclass VerboIrregular)
   if (WordInProperty(v, i, imperativo)) {
     if (i.&short_name~=0) return(PrintOrRun(i, short_name, 1));
     else { print (name) i; rtrue;}}
     
  rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageVerbIsDebugging is called by SearchScope.  It should return true
!  if word w is a debugging verb which needs all objects to be in scope.
! ----------------------------------------------------------------------------

#Ifdef DEBUG;
[ LanguageVerbIsDebugging w;
    if (w == 'xroba' or 'xarbol' or 'xmueve'
               or 'irdonde' or 'scope' or 'xobjeto' or
               'alcance' or 'xir')
        rtrue;
    rfalse;
];
#Endif;

! ----------------------------------------------------------------------------
!  LanguageVerbLikesAdverb is called by PrintCommand when printing an UPTO_PE
!  error or an inference message.  Words which are intransitive verbs, i.e.,
!  which require a direction name as an adverb ('walk west'), not a noun
!  ('I only understood you as far as wanting to touch /the/ ground'), should
!  cause the routine to return true.
! ----------------------------------------------------------------------------

[ LanguageVerbLikesAdverb w;
    if (w == 'vete' or 've' or 'camina' or
        'anda' or 'corre' or 'vuelve' or 'ir')
        rtrue;
    rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageVerbMayBeName is called by NounDomain when dealing with the
!  player's reply to a "Which do you mean, the short stick or the long
!  stick?" prompt from the parser. If the reply is another verb (for example,
!  LOOK) then then previous ambiguous command is discarded /unless/
!  it is one of these words which could be both a verb /and/ an
!  adjective in a 'name' property.
! ----------------------------------------------------------------------------

[ LanguageVerbMayBeName w j first_word;
    j=j;
    first_word=first_word;
    if (w == 'long' or 'short' or 'normal'
                    or 'brief' or 'full' or 'verbose' or 'saltar')
        rtrue;


    rfalse;
];

Constant NKEY__TX      = "S = Siguiente";
Constant PKEY__TX      = "    P=Previo";
Constant QKEY1__TX     = "Q=Volver al juego";
Constant QKEY2__TX     = "Q = Men� anterior";
Constant RKEY__TX      = "INTRO = leer este tema";

Constant NKEY1__KY     = 'S';
Constant NKEY2__KY     = 's';
Constant PKEY1__KY     = 'P';
Constant PKEY2__KY     = 'p';
Constant QKEY1__KY     = 'Q';
Constant QKEY2__KY     = 'q';

Constant SCORE__TX     = "Punt.: ";
Constant MOVES__TX     = "Movim.: ";
Constant TIME__TX      = "Hora: ";

Constant DARKNESS__TX  = "Oscuridad";

! En la revisi�n [020415] las constantes HAY*__TX se han trasladado al
! fichero Mensajes, pues dependen del tiempo, persona y n�mero verbales.
! INFSP - Se volvieron a su lugar, pues no se usa Mensajes.h

Constant THOSET__TX    = "esas cosas";
Constant THAT__TX      = "eso";
Constant OR__TX        = " o ";
Constant NOTHING__TX   = "nada";
Constant AND__TX       = " y ";
Constant WHOM__TX      = "que ";  ! Al parecer, aqu� sobraban los espacios
Constant WHICH__TX     = "que ";  ! de antes de los 'que'. [020423]
Constant COMMA__TX     = ", ";


! Funciones para manejar concordancias de g�nero y n�mero en los
! mensajes de respuesta gen�ricos.
!
! o -> escribe la terminaci�n -o -a -os -as seg�n el g�nero y n�mero
! del objeto recibido, para hacer concordar los adjetivos.
!
! n -> escribe la terminaci�n -"" -n seg�n el n�mero del objeto, para
! hacer concordar el verbo en plural.
!
! esta -> escribe "est�" o "est�n" seg�n el n�mero del objeto.
!
! del -> escribe "del" "de la" "de los" "de las" seg�n el g�nero y
! n�mero del objeto. En realidad, determina qu� art�culo habr�a de ir
! y si �ste es "el" realiza la contracci�n "del".
!
! al -> como del, pero con "al" "a la" "a los" "a las".
!
! lo -> Escribe "lo" "la" "le" "los" "las" "les" (proname) seg�n el
! g�nero y n�mero del objeto y seg�n sea animate o no.
!

[ o obj gna;

    gna=GetGNAOfObject(obj);
    switch(gna)
    {
     0,6: print "o";
     1,7: print "a";
     3,9: print "os";
     4,10: print "as";
    }
];    

[ e obj gna;

    gna=GetGNAOfObject(obj);
    switch(gna)
    {
     0,6: print "e";
     1,7: print "a";
     3,9: print "es";
     4,10: print "as";
    }
];


[ n obj;
    if (obj == player) print "s";
    else if (obj has pluralname) print "n";
];

[ s obj;
    if (obj has pluralname) print "s";
];

[ esta obj;
    print "est�", (n) obj;
];

[ es obj;
    if (obj has pluralname) print "son";
    else print "es";
];

[ _Es obj;
    if (obj has pluralname) print "Son";
    else print "Es";
];

[ _s obj;
    if (obj has pluralname) print "es";
];

[ el_ obj;
    if (obj hasnt proper) PrefaceByArticle(obj,1,-1);
];

[ un_ obj;
    if (obj hasnt proper) PrefaceByArticle(obj,2,-1);
];


[ el obj;
    print (the) obj;
];

[ _El obj;
    print (The) obj;
];

![ un obj;
!    if (un_(obj)) print " ";
!    print (name) obj;
!];

[ un obj;
!  if (obj has proper) print "a "; ![infsp] ya se incluy� esta linea en el hackeo de Indefart.
  print (a) obj;
];

[ _Un obj; ! indefinido con la primera letra en mayuscula. -Eliuk Blau
  print (A) obj;
];


[ _nombre_ obj;
    print (name) obj;
];

[ numero obj;
    print (number) obj;
];


! "al" y "del" plantean un bonito problema. La contracci�n debe
! producirse si el art�culo es "el", pero esto no se sabe consultando
! el GNA, pues hay palabras como "aguila" que son femeninas y no
! obstante usan "el" como art�culo.
! El truquillo consiste en llamar a (the) para averiguar qu� art�culo
! se imprimir�a (pero capturando la salida de ese print y guard�ndola
! en una variable). Si el art�culo en cuesti�n empieza por 'e', se
! produce la contracci�n.
!
! Para capturar la salida a una variable, es necesario descender al
! lenguaje ensamblador de la Z machine. Este truco lo aprend� del
! Designers manual (respuesta al ejercicio 96, p�gina 249)
!
!------------------------------------------------------------------------------
! ELIUK BLAU: He re-implementado la rutina para permitir que la primera letra
! se imprima en mayuscula si se activa el argumento 'flag' durante la llamada.
[ del_ obj flag; ! ELIUK: 'flag' es usado solo por rutina de impresion (_Del)
    ! ELIUK: aqui se decide si se pone mayuscula o no
    if (flag) { print "D"; } ! 'flag' activado
    else { print "d"; }      ! 'flag' desactivado

    if (obj has proper) {        ! 030305
      print "e "; ! si es nombre propio, poner 'e' final...
      rtrue;      ! ... y terminar la ejecucion aqui
    }

    #ifdef TARGET_ZCODE;
      texto_impreso-->0 = 50;
      @output_stream 3 texto_impreso;
      print (the)obj;
      @output_stream -3;
      if (texto_impreso->2 ~= 'e') { print "e "; } ! si NO ES articulo "el", agregar 'e' y un espacio
    #ifnot; !TARGET_GLULX
      PrintAnyToArray(texto_impreso, 50, PrefaceByArticle, obj, 1, -1);
      if (texto_impreso->0 ~= 'e') { print "e "; } ! si NO ES articulo "el", agregar 'e' y un espacio
    #endif;

    ! ahora imprimimos el articulo
    PrefaceByArticle(obj, 1, -1);
];

! Con "al" el truco es m�s complejo, ya que si el art�culo impreso por
! el_ empieza por "e", hay que cambiar esta "e" por una "a". Y si no
! empieza por "e" hay que poner una "a" delante.
! Adem�s debemos capturar el caso de que no se imprima art�culo alguno
! (si el objeto proporciona una propiedad "articulos" con cadenas
! vac�as).
!------------------------------------------------------------------------------
! ELIUK BLAU: He re-implementado la rutina para permitir que la primera letra
! se imprima en mayuscula si se activa el argumento 'flag' durante la llamada.
[ al_ obj flag; ! ELIUK: 'flag' es usado solo por rutina de impresion (_Al)
    #ifdef TARGET_ZCODE;
      texto_impreso-->0 = 50;
      @output_stream 3 texto_impreso;
      print (the) obj;
      @output_stream -3;
      ! print "^  ";for (i=2:i<50:i++) print (char)texto_impreso->i;print "   "; ! infsp debug
    #ifnot; !TARGET_GLULX
      ! PrintAnyToArray(texto_impreso, 50, PrefaceByArticle, obj, 1, -1);
      PrintAnyToArray(texto_impreso, 50, Defart, obj);! 070809 infsp fix: corrige mal funcionamiento con Propios - gracias Eliuk
      ! print "^  ";for (i=0:i<2:i++) print (char)texto_impreso->i;print "   "; ! infsp debug
    #endif; !TARGET_
    
    ! ELIUK: aqui se decide si se pone mayuscula o no
    if (flag) { print "A"; } ! 'flag' activado
    else { print "a"; }      ! 'flag' desactivado

    ! ELIUK: completamos el resto del texto que se imprimira finalmente
    #ifdef TARGET_ZCODE;
      if (texto_impreso->2 == 'e') { print "l "; }
      else {
        print " ";
        if (obj hasnt proper) { PrefaceByArticle(obj, 1, -1); }
      }
    #ifnot; !TARGET_GLULX
      if (texto_impreso->0 == 'e') { print "l "; }
      else {
        print " ";
        if (obj hasnt proper) { PrefaceByArticle(obj, 1, -1); }
      }
    #endif; !TARGET_
];

[ del obj;
    print (del_) obj;
    print (name) obj;
];

! ELIUK BLAU: Rutina de impresion "De", "Del", "De la", "De los", "De las" con la primera
! letra en mayuscula. Se usa como base el funcionamiento de la rutina del_() de INFSP.
[ _Del obj;
    del_(obj,1); ! con 1 se activa 'flag' para imprimir primera letra en mayuscula
    print (name) obj;
    ! NOTA: SI POSTERIORMENTE SE INTENTA MEJORAR ESTA RUTINA, JUNTO CON del_(),
    ! LA SOLUCION JAMAS DEBE SER IMPRIMIR A UN BUFFER PORQUE LOS RESULTADOS
    ! PUEDEN SER INESPERADOS. LA PROPIA del_() YA IMPRIME A UN BUFFER, ASI QUE
    ! OTRO TRABAJO DEL MISMO TIPO IMPLICARIA TENER QUE CREAR UN NUEVO ARRAY
    ! CON EL QUE TRABAJAR O USAR EL ARRAY COMUN StorageForShortName DE LA
    ! PROPIA LIBRERIA INFORM. EN ESTE ULTIMO CASO ES CUANDO SE PRODUCEN LOS
    ! PROBLEMAS... POR OTRO LADO: CREAR UN NUEVO ARRAY SOLO PARA QUE LO USE
    ! ESTA RUTINA NO TIENE SENTIDO. POR ESO LA NECESIDAD DEL 'flag' EN del_()
];

[ al obj;
    print (al_) obj;
    print (name) obj;
];

! ELIUK BLAU: Rutina de impresion "A", "Al", "A la", "A los", "A las" con la primera
! letra en mayuscula. Se usa como base el funcionamiento de la rutina al_() de INFSP.
[ _Al obj;
    al_(obj,1); ! con 1 se activa 'flag' para imprimir primera letra en mayuscula
    print (name) obj;
    ! NOTA: SI POSTERIORMENTE SE INTENTA MEJORAR ESTA RUTINA, JUNTO CON al_(),
    ! LA SOLUCION JAMAS DEBE SER IMPRIMIR A UN BUFFER PORQUE LOS RESULTADOS
    ! PUEDEN SER INESPERADOS. LA PROPIA al_() YA IMPRIME A UN BUFFER, ASI QUE
    ! OTRO TRABAJO DEL MISMO TIPO IMPLICARIA TENER QUE CREAR UN NUEVO ARRAY
    ! CON EL QUE TRABAJAR O USAR EL ARRAY COMUN StorageForShortName DE LA
    ! PROPIA LIBRERIA INFORM. EN ESTE ULTIMO CASO ES CUANDO SE PRODUCEN LOS
    ! PROBLEMAS... POR OTRO LADO: CREAR UN NUEVO ARRAY SOLO PARA QUE LO USE
    ! ESTA RUTINA NO TIENE SENTIDO. POR ESO LA NECESIDAD DEL 'flag' EN al_()
];

[ lo obj gna;
    gna=GetGNAOfObject(obj);
    switch(gna)
    {
     0,6: print "lo";
     1,7: print "la";
     3,9: print "los";
     4,10: print "las";
    }
];


! Cuando el usuario teclea un comando incompleto, es necesario que
! el parser genere un mensaje preguntando por lo que falta,
! pero la forma de preguntarlo depende del verbo. Por ej.:
!
! > coge
! �Qu� quieres coger?
!
! > mete llave
! �D�nde quieres meter la llave?
!
! > orco, saca la botella
! �De d�nde tiene que sacar el orco la botella?
!
! Cuando el parser tiene que imprimir una respuesta de estas,
! simplemente llama a MLIdioma con el n�mero de mensaje Misc, 48 o
! 49. En esta versi�n de la librer�a (6/7) es responsabilidad de
! LanguageLM generar el mensaje completo.
!
! En nuestro caso hemos implementado estos mensajes de forma que
! llaman a IniciarPregunta para que escriba una part�cula interrogativa
! como "Donde" "De donde", etc. Si no escribe nada, MLIdioma pone
! "Qu�" o "A qui�n" seg�n el tipo de error, a continuaci�n se escribe
! "quieres" o "tiene que" seg�n qui�n sea el actor. Se llama a
! LanguageVerb para que escriba el verbo, y finalmente se a�ade el
! name del objeto directo si lo hab�a, para terminar la pregunta
! cerrando la interrogaci�n.
!
! IniciarPregunta, lo que hace es: 
!
! 1) busca preposiciones en el patr�n que ha encajado, si las
! encuentra, la pregunta debe comenzar por alguna part�cula
! interrogativa. Por ejemplo, si la l�nea de gram�tica hallada era
! "saca" noun "de" noun, se hallar� la preposici�n "de" que nos indica
! que la pregunta ha de comenzar con "�De d�nde ...".
!
! 2) Llama a la funci�n "IdiomaPreguntarPrep" que recibe como
! par�metros la preposici�n y la acci�n en curso, para que imprima la
! part�cula interrogativa adecuada. Esta funci�n tan s�lo debe
! escribir esa part�cula.
!
! IdiomaPreguntarPrep llama a su vez a PreguntarPreposicion por si el
! programador a�ade verbos raros con preposiciones raras que no son
! correctamente manejados por esta rutina. Si PreguntarPreposicion
! existe y retorna distinto de cero, IdiomaPreguntarPrep asumir�
! que la part�cula interrogativa ya ha sido escrita por
! PreguntarPreposicion y no har� nada m�s.
!
[ IdiomaPreguntarPrep prepos action;

    if (PreguntarPreposicion(prepos, action)) return;
!    print "^IPPrep: action: ", (DebugAction)action, "  Prepos: ",(address)prepos,"^"; ! infsp debug 
    switch (prepos)
    {
     'de': print "De d�nde";
     'en', 'dentro', 'encima': print "D�nde";
     'bajo','debajo':print "Debajo de qu�";
     'sobre', 'acerca':print "Sobre qu�";
     'contra': print "Contra qu�";
     'hacia':print "Hacia d�nde";
     'con':if (action==##Tell or ##Attack)  print "Con qui�n";
              else print "Con qu�";
     'por':if (action==##Ask) print "Por qu�";
              else print "Por d�nde";
     'una', 'un' : print "A qui�n";
!     #n$a:  switch(action) ! esto es arcaico man!
     'a//':  switch(action){
           ##Go:                  print "Hacia d�nde";
           ##GoIn, ##Climb:       print "D�nde";
           ##ThrowAt, ##Give, ##Show, ##Answer, ##Tell, ##Ask:
                                  print "A qui�n";
           ##Search:              print "A trav�s de qu�";
           ##Tie:                 print "A qu�"; 
           ##Transfer,##Enter:    print "A d�nde";
           default:               print "A qui�n";
       };
    }
];

! A partir del patr�n que ha construido el parser, se determina cu�l
! es la primera preposici�n usada detr�s del verbo.
! Esto ser� usado por la funci�n IniciarPregunta y por LanguageIsVerb
! [INFSP] Recoding para un mejor comportamiento en caso de lineas de gramaticas complejas
! Ej: 'pon' noun 'en/sobre' noun . Antes usando 'sobre' (segundo token) no andaba.
! (devolvia NULL)

[ AveriguarPreposicion 
    i k p ;

!    print " ",pattern-->(pcount-1)," ^"; ![infsp] debug

    p = NULL;
    for (k=pcount:k>=1:k--){
!        print "^AP: k:",k; ! infsp debug
       i=pattern-->k;
!        print "^ i:",(address)No__Dword(i-REPARSE_CODE); ! infsp debug 
       if ( i == PATTERN_NULL) continue;
       if (i>=REPARSE_CODE)
           p=No__Dword(i-REPARSE_CODE);
    }
    return p;

];


[ AveriguarPrimeraPreposicion
    i k p found;
    p=NULL;
    for (k=pcount:k>=1:k--)
    { 
!     print "^AP: k:",k; ! infsp debug
     i=pattern-->k;
!    print "^ i:",i; ! infsp debug 
   if (pattern-->k == PATTERN_NULL) continue;
!    i=pattern-->k;
        if (i>=REPARSE_CODE)
        {
        i=No__Dword(i-REPARSE_CODE);
        p=i;    ! nos quedamos con la primera
        found=k;
        }
    }
!    print "APP: Primera prep: ",(address) p; ! infsp debug
!    print " / found :",found; ! infsp debug
!    print " / pcount :",pcount,"^"; ! infsp debug
    
    if (found==(pcount -1)) return NULL;
    else return p;
];


! Averigua si es necesaria una part�cula interrogativa. Llama a
! IdiomaPreguntarPrep para que la imprima.
!
! Retorna 1 si ha sido impresa la part�cula interrogativa, y 0 en caso
! contrario, (y entonces la funci�n que la llam� "LanguageLM", sabe
! que es necesario escribir "Qu�" o "A qui�n" ya que en este caso lo
! que faltaba era el objeto directo en lugar del complemento.
!
[ IniciarPregunta
    p;

    print "�";
    p=AveriguarPreposicion();
    if (p~=NULL) {
      IdiomaPreguntarPrep(p, action_to_be);
      return 1;
    }
    return 0;
    
];

[ IdiomaImprimirComando from i k spacing_flag prep;

  if (from==0)
  {   i=verb_word;
      LanguageVerb(i);
      from++; spacing_flag = true;
  }
  
! print "^Valor de from: ",from,"^"; ! infsp debug
! print "Valor de pcount: ",pcount,"^"; ! infsp debug
  
  for (k=from:k<pcount:k++){
     i=pattern-->k;
!     print "^IIC: i: ",  i, "^"; ! infsp debug
      if (i == PATTERN_NULL) continue;
!      if (spacing_flag) print (char) ' ';
      if (i == 0 ) { print " ",(string) THOSET__TX; jump TokenPrinted; }
      if (i == 1) { print " ",(string) THAT__TX;   jump TokenPrinted; }
      if (i >= REPARSE_CODE) continue;
      else {
            if (i in compass && LanguageVerbLikesAdverb(verb_word))
                LanguageDirection (i.door_dir); ! the direction name as adverb
            if (i==player) print "te";
            else  {
               prep=AveriguarPrimeraPreposicion();
               switch(prep){
                 'a//': print " ",(al) i;
                 'de': print " ",(del) i;
                 NULL: print " ",(the) i;
                 default: !print " ", (address) prep;
                           print " ",(the) i;
               }
            }
      continue;
      }
      .TokenPrinted;
      spacing_flag = true;
  } ! for
];


! [001115]
! Los mensajes de librer�a van ahora en fichero aparte
! [010515] Cambiado de sitio el punto de inclusi�n de Mensajes.h
! para dar la oportunidad al usuario de incluir su propio Mensajes
! si lo desea (antes del include Gramatica)
! Include "Mensajes";


[ LanguagePrintShortName obj aux;
  aux = aux;
  if (obj provides gender) {
    if (parser_trace >= 10)
      print "[",  obj, " tiene ~genero~=", obj.gender, ".]^";
    #ifdef DEBUG;
    aux = debug_flag; debug_flag = 0;
    #endif;
    switch (obj.gender) {
      0,1: give obj ~female ~pluralname; ! infsp fix for I7 compatibility (en I7 no se puede asignar el 0 a gender, de 1 para arriba si)
        2: give obj female ~pluralname;  ! print " FEMALE GIVEN "; !infsp debug
      4,5: give obj ~female pluralname;
        6: give obj female pluralname;
    }
    #ifdef DEBUG;
    debug_flag = aux;
    #endif;
    rfalse;
  } else {
!   if (parser_trace>=10)
!     print "[", obj, " no tiene la propiedad ~genero~.]";
      rfalse;
  }
];

! Esta funci�n de elegir objetos asigna m�s puntuaci�n al objeto que
! ha sido llamado por su 'name' (por tanto toman prioridad sobre los
! que son llamados solo por su 'adjetivo'). Se excluye al propio
! player de la lista, y los objetos de scenery. Solo para inform 6
! Inform 7 tiene su propio ChooseObjects, ver Cap 16.16
[ ChooseObjects obj codigo prio;
    
!    prio=ChooseObjects2(obj,codigo); ! [TODO] si ChooseObjects estuviera usada por otra libreria, retocar aqui
!    print "^[ElijeObjetos : priority = ",prio," (initial)"; ! infsp debug
    if (codigo>=2){
        if (obj has nombreusado){ !levantar prioridad al objeto mencionado en ultimo turno
           if (bandera_todo_vale==0) prio=prio+10;! bandera_todo_vale?????? infsp crazy freak question:de donde salio?
        }
        if ((obj == player)||((obj has scenery)&&(obj notin compass)))
           prio=prio-10;
    }
!    print "          priority = ",prio, " (last)"; ! infsp debug
    return prio;
];


!===============================================================================
! INICIO: Seccion Mensajes V0.8 Edicion Inicial 070905
!         Creada a partir de Mensajes.h (InformATE)
!
! NOTA DE ELIUK BLAU (a partir de R712.6):
! Si se define la constante de compilacion condicional SIN_MENSAJES,
! entonces no se incluiran en el codigo del juego los mensajes estandares
! de la libreria. De esta manera se le permite al programador implementar
! sus propios mensajes de la libreria en el codigo del juego o mediante
! alguna libreria de extension. Recuerda que si solo vas a redefinir unos
! pocos mensajes y no todos es siempre mejor usar el mecanismo de
! "LibraryMessages". Consulta el manual de Inform para mayores detalles.
!-------------------------------------------------------------------------------
#ifdef SIN_MENSAJES;
  Message "> Spanish: AVISO - Omitiendo los mensajes estandares de la libreria";
#ifnot; 

Constant PRIMERA_PERSONA  = $$00001;
Constant SEGUNDA_PERSONA  = $$00010;
Constant TERCERA_PERSONA  = $$00100;
Constant PERSONA_SINGULAR = $$01000;
Constant PERSONA_PLURAL   = $$10000;
! Para los switch:
Constant PRIMERA_PERSONA_SINGULAR = PRIMERA_PERSONA + PERSONA_SINGULAR;
Constant SEGUNDA_PERSONA_SINGULAR = SEGUNDA_PERSONA + PERSONA_SINGULAR;
Constant TERCERA_PERSONA_SINGULAR = TERCERA_PERSONA + PERSONA_SINGULAR;
Constant PRIMERA_PERSONA_PLURAL   = PRIMERA_PERSONA + PERSONA_PLURAL;
Constant SEGUNDA_PERSONA_PLURAL   = SEGUNDA_PERSONA + PERSONA_PLURAL;
Constant TERCERA_PERSONA_PLURAL   = TERCERA_PERSONA + PERSONA_PLURAL;

Property persona alias number;

[ AsignarPersona per;
  player.persona = per;
  if (player provides persona) {
    if (player.persona & TERCERA_PERSONA) {
      if (player.persona & PERSONA_PLURAL) {
        FORMER__TX    = "su antiguo ~yo~";
        CANTGO__TX    = "No pueden ir por ah�.";
        IS__TX        = " ven";
        ARE__TX       = " ven";
        IS2__TX       = "ven ";
        ARE2__TX      = "ven ";
        PARTICULA_TE  = "se";
        if (player has female || (player provides gender && player.gender & G_FEMENINO)) {
          YOURSELF__TX  = "ellas mismas";
          YOU__TX       = "Ellas";
        } else {
          YOURSELF__TX  = "ellos mismos";
          YOU__TX       = "Ellos";
        }
      } else {
        FORMER__TX    = "su antiguo ~yo~";
        CANTGO__TX    = "No puede ir por ah�.";
        IS__TX        = " ve";
        ARE__TX       = " ve";
        IS2__TX       = "ve ";
        ARE2__TX      = "ve ";
        PARTICULA_TE  = "le";
        if (player has female || (player provides gender && player.gender & G_FEMENINO)) {
          YOURSELF__TX  = "ella misma";
          YOU__TX       = "Ella";
        } else {
          YOURSELF__TX  = "�l mismo";
          YOU__TX       = "�l";
        }
      }
    } else if (player.persona & PRIMERA_PERSONA) {
      if (player.persona & PERSONA_PLURAL) {
        FORMER__TX    = "nuestro antiguo ~yo~";
        CANTGO__TX    = "No podemos ir por ah�.";
        IS__TX        = " vemos";
        ARE__TX       = " vemos";
        IS2__TX       = "vemos ";
        ARE2__TX      = "vemos ";
        PARTICULA_TE  = "nos";
        if (player has female || (player provides gender && player.gender & G_FEMENINO)) {
          YOURSELF__TX  = "nosotras mismas";
          YOU__TX       = "Nosotras";
        } else {
          YOURSELF__TX  = "nosotros mismos";
          YOU__TX       = "Nosotros";
        }
      } else {
        FORMER__TX    = "mi antiguo ~yo~";
        CANTGO__TX    = "No puedo ir por ah�.";
        IS__TX        = " veo";
        ARE__TX       = " veo";
        IS2__TX       = "veo ";
        ARE2__TX      = "veo ";
        PARTICULA_TE  = "me";
        if (player has female || (player provides gender && player.gender & G_FEMENINO)) {
          YOURSELF__TX  = "nosotras mismas";
          YOU__TX       = "Nosotras";
        } else {
          YOURSELF__TX  = "nosotros mismos";
          YOU__TX       = "Nosotros";
        }      
      }
    } else { ! Segunda persona
      if (player.persona & PERSONA_PLURAL) {
        FORMER__TX    = "vuestro antiguo ~yo~";
        CANTGO__TX    = "No pod�is ir por ah�.";
        IS__TX        = " veis";
        ARE__TX       = " veis";
        IS2__TX       = "veis ";
        ARE2__TX      = "veis ";
        PARTICULA_TE  = "os";
        if (player has female || (player provides gender && player.gender & G_FEMENINO)) {
          YOURSELF__TX  = "vosotras mismas";
          YOU__TX       = "Vosotras";
        } else {
          YOURSELF__TX  = "vosotros mismos";
          YOU__TX       = "Vosotros";
        }      
      } else {
.SegundaSingular;
        FORMER__TX    = "tu antiguo ~yo~";
        CANTGO__TX    = "No puedes ir por ah�.";
        IS__TX        = " ves";
        ARE__TX       = " ves";
        IS2__TX       = "ves ";
        ARE2__TX      = "ves ";
        PARTICULA_TE  = "te";
        if (player has female || (player provides gender && player.gender & G_FEMENINO)) {
          YOURSELF__TX  = "ti misma";
          YOU__TX       = "T�";
        } else {
          YOURSELF__TX  = "ti mismo";
          YOU__TX       = "T�";
        }            
      }
    }
  } else jump SegundaSingular;
  ChangeDefault(cant_go, CANTGO__TX);
];

[ ti_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "m�";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "ti";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "s�";
      PRIMERA_PERSONA_PLURAL:   print "nosotr";
        if (player has female || (player provides gender &&
            player.gender & G_FEMENINO))  print "as";
        else                              print "os";
      SEGUNDA_PERSONA_PLURAL:   print "vosotr";
        if (player has female || (player provides gender &&
            player.gender & G_FEMENINO))  print "as";
        else                              print "os";
      TERCERA_PERSONA_PLURAL:   print "ell";
        if (player has female || (player provides gender &&
            player.gender & G_FEMENINO))  print "as";
        else                              print "os";
      default:                  print "ti";
    }
  } else print "ti";
  print (string) x;
];

[ se_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "me";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "te";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "se";
      PRIMERA_PERSONA_PLURAL:   print "nos";
      SEGUNDA_PERSONA_PLURAL:   print "os";
      TERCERA_PERSONA_PLURAL:   print "se";
      default:                  print "te";
    }
  } else print "te";
  print (string) x;
];

[ te_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "me";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "te";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "le";
      PRIMERA_PERSONA_PLURAL:   print "nos";
      SEGUNDA_PERSONA_PLURAL:   print "os";
      TERCERA_PERSONA_PLURAL:   print "les";
      default:                  print "te";
    }
  } else print "te";
  print (string) x;
];

[ _Te_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "Me";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "Te";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "Le";
      PRIMERA_PERSONA_PLURAL:   print "Nos";
      SEGUNDA_PERSONA_PLURAL:   print "Os";
      TERCERA_PERSONA_PLURAL:   print "Les";
      default:                  print "Te";
    }
  } else print "Te";
  print (string) x;
];

[ tu_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "mi";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "tu";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "su";
      PRIMERA_PERSONA_PLURAL:   print "nuestr";
        if (player has female || (player provides gender &&
            player.gender & G_FEMENINO))  print "a";
        else                              print "o";
      SEGUNDA_PERSONA_PLURAL:   print "vuestr";
        if (player has female || (player provides gender &&
            player.gender & G_FEMENINO))  print "a";
        else                              print "o";
      TERCERA_PERSONA_PLURAL:   print "su";
      default:                  print "tu";
    }
  } else print "tu";
  print (string) x;
];

[ _Tu_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "Mi";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "Tu";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "Su";
      PRIMERA_PERSONA_PLURAL:   print "Nuestr";
        if (player has female || (player provides gender &&
            player.gender & G_FEMENINO))  print "a";
        else                              print "o";
      SEGUNDA_PERSONA_PLURAL:   print "Vuestr";
        if (player has female || (player provides gender &&
            player.gender & G_FEMENINO))  print "a";
        else                              print "o";
      TERCERA_PERSONA_PLURAL:   print "Su";
      default:                  print "tu";
    }
  } else print "tu";
  print (string) x;
];

[ as_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "o";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "as";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "a";
      PRIMERA_PERSONA_PLURAL:   print "amos";
      SEGUNDA_PERSONA_PLURAL:   print "�is";
      TERCERA_PERSONA_PLURAL:   print "an";
      default:                  print "as";
    }
  } else print "as";
  print (string) x;
];

[ es_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "o";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "es";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "e";
      PRIMERA_PERSONA_PLURAL:   print "emos";
      SEGUNDA_PERSONA_PLURAL:   print "�is";
      TERCERA_PERSONA_PLURAL:   print "en";
      default:                  print "es";
    }
  } else print "es";
  print (string) x;
];

[ _es_ x; ! Ya existe una rutina llamada 'es_'
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "o";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "es";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "e";
      PRIMERA_PERSONA_PLURAL:   print "imos";
      SEGUNDA_PERSONA_PLURAL:   print "�s";
      TERCERA_PERSONA_PLURAL:   print "en";
      default:                  print "es";
    }
  } else print "es";
  print (string) x;
];

[ oy_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "oy";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "�s";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "a";
      PRIMERA_PERSONA_PLURAL:   print "amos";
      SEGUNDA_PERSONA_PLURAL:   print "�is";
      TERCERA_PERSONA_PLURAL:   print "�n";
      default:                  print "�s";
    }
  } else print "�s";
  print (string) x;
];

[ as__ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "�";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "�s";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "�";
      PRIMERA_PERSONA_PLURAL:   print "emos";
      SEGUNDA_PERSONA_PLURAL:   print "�is";
      TERCERA_PERSONA_PLURAL:   print "�n";
      default:                  print "�s";
    }
  } else print "�s";
  print (string) x;
];

[ a_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "a";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "as";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "a";
      PRIMERA_PERSONA_PLURAL:   print "amos";
      SEGUNDA_PERSONA_PLURAL:   print "�is";
      TERCERA_PERSONA_PLURAL:   print "an";
      default:                  print "as";
    }
  } else print "as";
  print (string) x;
];

[ eo_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "eo";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "es";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "e";
      PRIMERA_PERSONA_PLURAL:   print "emos";
      SEGUNDA_PERSONA_PLURAL:   print "�is";
      TERCERA_PERSONA_PLURAL:   print "en";
      default:                  print "es";
    }
  } else print "es";
  print (string) x;
];

[ e_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "e";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "as";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "a";
      PRIMERA_PERSONA_PLURAL:   print "emos";
      SEGUNDA_PERSONA_PLURAL:   print "ab�is";
      TERCERA_PERSONA_PLURAL:   print "an";
      default:                  print "as";
    }
  } else print "as";
  print (string) x;
];

[ os_ x;
  if (player provides persona) {
    if (player.persona & PERSONA_PLURAL) print "os";
    else                                 print "o";
  } else { ! Por defecto, singular
    print "o";
  }
  print (string) x;
];

[ z_ x;
  if (player provides persona) {
    if (player.persona & PERSONA_PLURAL) print "ces";
    else                                 print "z";
  } else { ! Por defecto, singular
    print "z";
  }
  print (string) x;
];

[ ias_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "�a";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "�as";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "�a";
      PRIMERA_PERSONA_PLURAL:   print "�amos";
      SEGUNDA_PERSONA_PLURAL:   print "�ais";
      TERCERA_PERSONA_PLURAL:   print "�an";
      default:                  print "�as";
    }
  } else print "�as";
  print (string) x;
];

[ ues_ x;
  if (player provides persona &&
      player.persona & PRIMERA_PERSONA) print "o", (string) x;
  else                                  print "u", (_es_) x;
];

[ sales_ x;
  print "sal";
  if (player provides persona && player.persona ==
      PRIMERA_PERSONA or PRIMERA_PERSONA_SINGULAR) print "g";
  print (_es_) x;
];

[ _Sales_ x;
  print "Sal";
  if (player provides persona && player.persona ==
      PRIMERA_PERSONA or PRIMERA_PERSONA_SINGULAR) print "g";
  print (_es_) x;
];

[ pones_ x;
  print "pon";
  if (player provides persona && player.persona ==
      PRIMERA_PERSONA or PRIMERA_PERSONA_SINGULAR) print "g";
  print (es_) x;
];

[ tienes_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "teng";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "tien";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "tien";
      PRIMERA_PERSONA_PLURAL:   print "tene";
      SEGUNDA_PERSONA_PLURAL:   print "ten";
      TERCERA_PERSONA_PLURAL:   print "tien";
      default:                  print "tien";
    }
  } else print "tien";
  print (es_) x;
];

[ _Tienes_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "Teng";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "Tien";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "Tien";
      PRIMERA_PERSONA_PLURAL:   print "Tene";
      SEGUNDA_PERSONA_PLURAL:   print "Ten";
      TERCERA_PERSONA_PLURAL:   print "Tien";
      default:                  print "Tien";
    }
  } else print "Tien";
  print (es_) x;
];

[ sueltes_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "suelte";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "sueltes";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "suelte";
      PRIMERA_PERSONA_PLURAL:   print "soltemos";
      SEGUNDA_PERSONA_PLURAL:   print "solt�is";
      TERCERA_PERSONA_PLURAL:   print "suelten";
      default:                  print "sueltes";
    }
  } else print "sueltes";
  print (string) x;
];

[ _Cierras_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "Cierro";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "Cierras";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "Cierra";
      PRIMERA_PERSONA_PLURAL:   print "Cerramos";
      SEGUNDA_PERSONA_PLURAL:   print "Cerr�is";
      TERCERA_PERSONA_PLURAL:   print "Cierran";
      default:                  print "Cierras";
    }
  } else print "Cierras";
  print (string) x;
];

[ puedes_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "puedo";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "puedes";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "puede";
      PRIMERA_PERSONA_PLURAL:   print "podemos";
      SEGUNDA_PERSONA_PLURAL:   print "pod�is";
      TERCERA_PERSONA_PLURAL:   print "pueden";
      default:                  print "puedes";
    }
  } else print "puedes";
  print (string) x;
];

[ _Puedes_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "Puedo";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "Puedes";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "Puede";
      PRIMERA_PERSONA_PLURAL:   print "Podemos";
      SEGUNDA_PERSONA_PLURAL:   print "Pod�is";
      TERCERA_PERSONA_PLURAL:   print "Pueden";
      default:                  print "Puedes";
    }
  } else print "Puedes";
  print (string) x;
];

[ pueda_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "pueda";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "puedas";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "pueda";
      PRIMERA_PERSONA_PLURAL:   print "podamos";
      SEGUNDA_PERSONA_PLURAL:   print "pod�is";
      TERCERA_PERSONA_PLURAL:   print "puedan";
      default:                  print "puedas";
    }
  } else print "puedas";
  print (string) x;
];

[ sientes_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "siento";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "sientes";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "siente";
      PRIMERA_PERSONA_PLURAL:   print "sentimos";
      SEGUNDA_PERSONA_PLURAL:   print "sent�s";
      TERCERA_PERSONA_PLURAL:   print "sienten";
      default:                  print "sientes";
    }
  } else print "sientes";
  print (string) x;
];

[ encuentro_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "encuentro";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "encuentras";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "encuentra";
      PRIMERA_PERSONA_PLURAL:   print "encontramos";
      SEGUNDA_PERSONA_PLURAL:   print "encontr�is";
      TERCERA_PERSONA_PLURAL:   print "encuentran";
      default:                  print "encuentro";
    }
  } else print "encuentro";
  print (string) x;
];

[ _se_ x;
  if (player provides persona) {
    if (player.persona == PRIMERA_PERSONA or
        PRIMERA_PERSONA_SINGULAR) print "s�", (string) x;
    else                                    print "sab", (es_) x;
  } else print "s�", (string) x;
];

[ eres_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "soy";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "eres";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "es";
      PRIMERA_PERSONA_PLURAL:   print "somos";
      SEGUNDA_PERSONA_PLURAL:   print "sois";
      TERCERA_PERSONA_PLURAL:   print "son";
      default:                  print "eres";
    }
  } else print "eres";
  print (string) x;
];

[ _Eres_ x;
  if (player provides persona) {
    switch (player.persona) {
      PRIMERA_PERSONA,
      PRIMERA_PERSONA_SINGULAR: print "Soy";
      SEGUNDA_PERSONA,
      SEGUNDA_PERSONA_SINGULAR: print "Eres";
      TERCERA_PERSONA,
      TERCERA_PERSONA_SINGULAR: print "Es";
      PRIMERA_PERSONA_PLURAL:   print "Somos";
      SEGUNDA_PERSONA_PLURAL:   print "Sois";
      TERCERA_PERSONA_PLURAL:   print "Son";
      default:                  print "Eres";
    }
  } else print "Eres";
  print (string) x;
];

! ti_       se_  te_  tu_      as_    es_   _es_  oy_   as__  a_    eo_   e_     os_   z_   ias_   ues_
! _________________________________________________________________________________________________________
! m�        me   me   mi       o      o     o     oy    �     a     eo    e      o     z    �a     o
! ti        te   te   tu       as     es    es    �s    �s    as    es    as     o     z    �as    ues
! s�        se   le   su       a      e     e     a     �     a     e     a      o     z    �a     ue
! nosotr@s  nos  nos  nuestr@  amos   emos  imos  amos  emos  amos  emos  emos   os    ces  �amos  uimos
! vosotr@s  os   os   vuestr@  �is    �is   �s    �is   �is   �is   �is   ab�is  os    ces  �ais   u�s
! ell@s     se   les  su       an     en    en    �n    �n    an    en    an     os    ces  �an    uen

[ LanguageLM n x1;
! Answer: "", (The) second , " no ", (te_) " responde."; ! esto sera s�lo para I6?
  Answer, Ask: "No hay respuesta.";
! Ask:    "", (The) x1 , " no ", (te_) " responde.";
! Ask:    see Answer
  Attack: "La violencia no es la soluci�n.";
  Blow:   "", (_Tu_) " soplido no produce ning�n efecto.";
  Burn:   "Con esa peligrosa acci�n no lograr", (ias_) " nada.";
  Buy:    "No hay nada en venta.";
  Climb:  "No creo que vay", (a_) " a lograr nada as�.";
  Close:  switch (n) {
            1: "No es algo que pueda cerrarse.";
            2: "Ya estaba", (n) x1, " cerrad", (o) x1, ".";
            3: "", (_Cierras_) " ", (the) x1, ".";
          }
  CommandsOff: switch (n) { ![6/11]
                 1: "[Grabaci�n de �rdenes finalizada.]";
                 #Ifdef TARGET_GLULX;
                 2: "[La grabaci�n de �rdenes ya estaba finalizada.]";
                 #Endif; ! TARGET_
               }
  CommandsOn:  switch (n) { ! [6/11]
                 1: "[Grabaci�n de �rdenes activada.]";
                 #Ifdef TARGET_GLULX;
                 2: "[�rdenes relanzadas.]";
                 3: "[Grabaci�n de �rdenes ya activada.]";
                 4: "[Fallo en la grabaci�n de �rdenes.]";
                 #Endif; ! TARGET_
               }
  CommandsRead: switch (n) { ! [6/11]
                  1: "[Relanzando acciones.]";
                  #Ifdef TARGET_GLULX;
                  2: "[Las �rdenes ya est�n siendo relanzadas.]";
                  3: "[Fallo en la reejecuci�n de �rdenes: grabaci�n en curso.]";
                  4: "[Fallo en la reejecuci�n de �rdenes.]";
                  5: "[Acciones ejecutadas: fin del proceso.]";
                  #Endif; ! TARGET_
                }
  Consult:  "No descubr", (_es_) " nada interesante en ", (the) x1, " sobre ese tema.";
  Cut:      "Cort�ndo", (lo) x1, " no lograr", (as__) " gran cosa.";
  Dig:      "Excavar no servir� de nada aqu�.";
  Disrobe:   switch (n) {
               1: "No llev", (as_) " puesto eso.";
               2: print_ret (_Te_) " quit", (as_) " ", (the) x1, ".";
    }
  Drink:    "Eso no parece potable.";
  Drop:
    ! 2 mensajes de error, 1 aviso y 1 de �xito:
    !   1: Error, el objeto dejado no est� en poder del jugador, pero
    !      est� en la localidad.
    !   2: Error, el objeto dejado no est� en poder del jugador ni en
    !      la localidad.
    !   3: Aviso, el objeto era una prenda que el jugador llevaba
    !      puesta y la librer�a va a quitarsela autom�ticamente para
    !      poder dejarla.
    !   4: Exito
    switch (n) {
     1: if (x1 has animate)
          "Para dejar ", (al) x1, " deber", (ias_) " tener", (lo) x1, ".";
        "Para dejar ", (the) x1, " deber", (ias_) " tener", (lo) x1, ".";
     2: "No ", (lo) x1, " ", (tienes_) ".";
     3: "(primero ", (te_) "quit", (as_) " ", (the) x1, ")";
     4: "Dejad", (o) x1, ".";
    }
  Eat:
    !  1: Error, el objeto no tiene el atributo "comestible"
    !  2: Exito. [NOTA: la librer�a permite por defecto comerse
    !    cualquier cosa que tenga el atributo "comestible", pero la
    !    �nica acci�n que causa esto es que el objeto en cuesti�n
    !    desaparece del juego al ser comido]
    switch (n) {
      1: "Eso es simplemente incomestible.";
      2: "", (_Te_) " com", (es_) " ", (the) x1, ". No est� mal.";
    }
  EmptyT:
    ! 1: Error, el jugador intenta vaciar un objeto en otro que no es
    !    un recipiente.
    ! 2: Error, el jugador intenta vaciar un objeto sobre un
    !    recipiente cerrado.
    ! 3: Error, el jugador intenta vaciar un objeto que no tiene nada
    !    dentro.
    ! 4: Error, el jugador intenta vaciar un objeto sobre s� mismo.
    ! [NOTA: No hay mensajes de �xito para esta acci�n, ya que en caso
    ! de haber superado los test anteriores, la librer�a genera
    ! finalmente la acci�n Transferir, la cual a su vez, generar� la
    ! acci�n PonerSobre o Meter, seg�n el segundo objeto sea soporte o
    ! recipiente. Por tanto los mensajes de �xito ser�n los de las
    ! acciones PonerSobre o Meter.
    switch (n) {
      1: "", (The) x1, " no puede tener cosas dentro.";
      2: "", (The) x1, " ", (esta) x1, " cerrad", (o) x1, ".";
      3: "", (The) x1, " ya ", (esta) x1, " vac�", (o) x1, ".";
      4: "No ", (puedes_) " vaciar un objeto sobre s� mismo."; ! [INFSP] antes usaba x1
    }
  Enter: ! [infsp] GoIn (Meterse) se cambio por Enter.
    ! CUIDADO. La acci�n Meterse se genera en muy diversas
    ! circunstancias: VETE HACIA EL PUENTE, VETE AL PUENTE, VETE POR
    ! EL PUENTE, PASA POR EL PUENTE, ENTRA EN EL PUENTE, ENTRA AL
    ! PUENTE, CRUZA EL PUENTE, CRUZA POR EL PUENTE, METETE EN EL
    ! PUENTE, METETE POR EL PUENTE, ATRAVIESA EL PUENTE, BAJA POR EL
    ! PUENTE.  Todas ellas generar�an la acci�n <Meterse Puente>
    !
    ! Adem�s, los intentos de entrar en un "entrable", tambi�n, como
    ! por ejemplo: SIENTATE EN EL BANCO, ECHATE EN EL BANCO, SUBETE AL
    ! BANCO, SUBETE EN EL BANCO, SUBE AL BANCO, SUBE POR EL BANCO,
    ! SALTA AL BANCO. Todas ellas generar�an <Meterse Banco>
    !
    ! Puesto que hay muchos verbos diferentes que dan lugar a la misma
    ! acci�n, es necesaria alguna forma de imprimir qu� verbo concreto
    ! ha sido el que la ha desencadenado, para poder generar mensajes
    ! como "No puedes entrar ahi", "No puedes sentarte ahi", "No
    ! puedes subirte ahi", etc.. seg�n el verbo usado. Para esto puede
    ! usarse una llamada a IdiomaVerbo(palabra_verbo). palabra_verbo
    ! contiene el verbo usado por el jugador (convertido en forma
    ! imperativa), y IdiomaVerbo lo que hace es imprimir la forma
    ! infinitiva.
    !
    ! Mensajes que puede generar esta acci�n:
    !  1: Error, el jugador ya est� en/sobre el objeto en cuesti�n
    !  2: Error, el objeto al que intenta entrar/subirse/meterse no
    !     tiene el atributo "entrable"
    !  3: Error, el objeto en que intenta entrar/meterse/subirse, es
    !     un entrable y recipiente, pero est� cerrado.
    !  4: Error, el objeto en que intenta entrar/meterse/subirse, est�
    !     en poder del jugador
    !  5: Exito, el jugador sube/entra en el objeto
    !  6: Aviso, el jugador estaba en otro objeto, la librer�a
    !     intentar� una acci�n <Salir> autom�tica, antes de reintentar lo
    !     que el jugador le ha ordenado.
    !  7: Aviso, el objeto en que el jugador intenta meterse, est�
    !     dentro de otros objetos. La librer�a intentar� de forma
    !     autom�tica meter al jugador en el objeto desde el cual pueda
    !     acceder al que el jugador ha indicado.
    switch(n) {
      1: print "Pero si ya est", (oy_) " ";
         if (x1 has supporter) print "sobre "; else print "en ";
         "", (the) x1, ".";
      2: print "No es algo por donde ", (pueda_) " "; LanguageVerb(verb_word); ".";
      3: "No ", (puedes_) " entrar en ", (the) x1, " porque est�", (n) x1,
         " cerrad", (o) x1, ".";
      4: print "No ", (puedes_) " "; LanguageVerb(verb_word);
         " ah� mientras no lo ", (sueltes_) ".";
      5: if (x1 has supporter) print "Sub", (_es_) " ", (al) x1;
         else                  print "Entr", (as_) " en ", (the) x1;
         ".";
      6: if (x1 has supporter) print "(", (te_) " baj", (as_) " ";
         else                  print "(", (sales_)  " ";
         print (del) x1; ")";
      7: if (x1 has supporter) "(", (te_) " sub", (_es_) " ", (al) x1, ")^";
         if (x1 has container) "(", (te_) " met", (es_) " en ", (the) x1, ")^";
         "(entr", (as_) " en ", (the) x1, ")^";
    }
    
  Examine:
    !  1: Error, el jugador intenta examinar un objeto, pero est� a
    !     oscuras.
    !  2: Exito, pero el objeto examinado no tiene descripcion
    !  3: Exito, pero el objeto examinado no tiene descripcion, aunque
    !     tiene el atributo conmutable, por lo que la librer�a genera
    !     este mensaje para indicar si est� apagado o encendido.
    switch (n) {
      1: "Est� demasiado oscuro, no ", (puedes_) " ver nada.";
      2: "No observ", (as_) " nada especial en ", (the) x1, ".";
      3: print (The) x1, " ", (esta) x1;
         if (x1 has on) " encendid", (o)x1, ".";
         else " apagad", (o)x1,".";
    }

  Exit:
    ! 1: Error, el jugador no est� subido/sentado/metido en ning�n

    !    objeto. [Nota, si la localidad tiene la propiedad "afuera",
    !    entonces la acci�n Salir lo sacar� de la localidad. En caso
    !    contrario es cuando se genera este error]
    ! 2: Error, el jugador est� dentro de un recipiente cerrado.
    ! 3: Exito, el jugador sale/baja del objeto
    ! 4: Error, mas especifico. [6/11]
    switch (n) {
      1: print "No est", (oy_) " en ning�n sitio del que deb", (a_) " ";
         LanguageVerb(verb_word); ".";
      2: "No ", (puedes_) " salir ", (del) x1, " porque est� cerrad", (o) x1, ".";
      3: if (x1 has supporter) print "Baj", (as_) " ";
         else                  print (_Sales_) " ";
         print_ret (del) x1, ".";
     4:  print "Pero es que no est", (oy_) " ";
         if (x1 has supporter) print "encima ";
         else                  print "dentro ";
         print_ret (del) x1, ".";
    }

  Fill:  "No ", (puedes_) " llenar", (lo) x1, ".";
  FullScore:
    ! Puntuaci�n en modo "explicativo". Puede activarse porque el
    ! jugador lo pida con el verbo "PUNTUACION LARGA" o porque el
    ! juego haya terminado (y la constante HAY_TAREAS est� definida).
    !
    !  1: texto introductorio al desglose de puntos
    !  2: texto de objetos importantes (los que tienen el atributo
    !     "valepuntos")
    !  3: texto de lugares importantes (los que tienen el atributo
    !     "valepuntos")
    !  4: texto final, tras la suma total impresa por la librer�a
    switch (n) {
      1: if (deadflag) print "La puntuaci�n se desglos� ";
         else          print "La puntuaci�n se desglosa ";
         "de la siguiente manera:^";
      2: "por encontrar objetos importantes";
      3: "por visitar lugares importantes";
      4: print "total (de ", MAX_SCORE ; ")";
    }
  GetOff:
    ! Esta acci�n se genera ante la frase SAL DEL ARMARIO o
    ! similares. Ante SAL a secas la acci�n es Salir.
    ! Si el jugador no est� en el objeto indicado, se genera el
    ! siguiente mensaje de error. Si est�, la librer�a generar� una
    ! acci�n Salir, por lo que el mensaje de �xito ser� el de Salir.
    "Pero si no est", (oy_) " en ", (the) x1, ".";
  Give:
    ! 1: Error, el jugador intenta dar un objeto que no tiene
    ! 2: Error, el jugador se da el objeto a s� mismo
    ! 3: Aviso, se ha enviado la acci�n Dar al PNJ, pero este no ha
    !    respondido. En este caso ml_o apunta al PNJ y no al objeto
    !    que le hemos dado.
    ! [Observar que por defecto el objeto sigue en poder del
    ! jugador. Si queremos que el PNJ lo acepte, hay que programarlo
    ! como parte de su rutina Vida]
    switch (n) {
      1: "No ", (tienes_) " ", (the) x1, ".";
      2: "Manose", (as_) " ", (the) x1,
         " un ratito, pero no consigu", (_es_) " gran cosa.";
      3: print_ret (The) x1, " no parece", (n) x1, " interesad", (o)x1, ".";
    }
  Go:
    ! La acci�n Ir se genera si el jugador pone IR NORTE, o si pone
    ! simplemente NORTE.
    !   1: Error, el jugador est� dentro/subido en un objeto del que
    !      tiene que salir antes
    !   2: Error, la localidad no tiene salida en esa direcci�n (y
    !      tampoco tiene la propiedad no_puedes_ir, ya que en este caso
    !      se habr�a impreso el valor de esa propiedad
    !   3: Error, el jugador ha intentado ir hacia arriba, pero all�
    !      hay una puerta cerrada
    !   4: Error, el jugador ha intentado ir hacia abajo, pero alli
    !      hay una puerta cerrada.
    !   5: Error, en la direcci�n que ha dicho el jugador hay una
    !      puerta cerrada
    !   6: Error, en la direcci�n que ha dicho el jugador hay una
    !      puerta, pero �sta no tiene una propiedad puerta_a
    !      (probablemente un error de programaci�n del juego)
    switch (n) {
      1: print (_Tienes_) " que ";
         if (x1 has supporter) print "bajar", (te_) " "; else print "salir ";
         print_ret (del) x1, " antes.";
      2: "No ", (puedes_) " ir por ah�.";
      3: "", (_Eres_) " incapaz de trepar por ", (the) x1, ".";
      4: "", (_Eres_) " incapaz de bajar por ", (the) x1, ".";
      5: "No ", (puedes_) " pasar a trav�s ", (del) x1, ".";
      6: "No ", (puedes_) " ir porque ", (the) x1, " no lleva", (n) x1, " a ning�n sitio.";
    }
  Insert:
    ! 1: Error, el objeto no est� en poder del jugador. [Nota,
    !    conviene mirar en este caso si el objeto es animado o no,
    !    para generar un mensaje m�s adecuado]
    ! 2: Error, el jugador intenta meter el objeto en otro que no
    !    tiene el atributo "recipiente"
    ! 3: Error, el jugador intenta meter el objeto en un recipiente
    !    cerrado.
    ! 4: Error, el objeto es una prenda que el jugador lleva puesta.
    !    [ESTE MENSAJE NO SE GENERA NUNCA, PARECE UN BUG DE LA
    !    LIBRERIA. V�ase en su lugar el mensaje 6]
    ! 5: Error, se intenta poner un objeto dentro de s� mismo
    ! 6: Aviso, el objeto es una prenda puesta, la librer�a va a
    !    quit�rsela de forma autom�tica antes de reintentar Meter.
    ! 7: Error, no queda sitio en el recipiente (el n�mero de objetos
    !    que un recipiente puede contener se indica en su propiedad
    !    "capacidad").
    ! 8: Exito. Mensaje a mostrar para cada objeto metido cuando se
    !    meten muchos (ej: METE TODO EN LA BOLSA)
    ! 9: Exito. Mensaje a mostrar cuando se mete un solo objeto dentro
    !    de otro
    switch (n) {
      1: if (x1 has animate) "Antes tendr�", (a_) " que ", (coge)"rl", (o) x1,
                             " y no s� si se dejar�",(n)x1,".";
         else                "Necesit", (as_) " tener ", (the) x1,
                             " para poder meter", (lo) x1,
                             " donde sea.";
      2: "No se pueden meter cosas dentro ", (del) x1, ".";
      3: print_ret (The) x1, " ", (esta) x1, " cerrad", (o) x1, ".";
      4: "", (_Tienes_) " que quit�r", (te_) "", (lo) x1, " antes.";
      5: "No ", (puedes_) " poner un objeto dentro de s� mismo.";
      6: "(primero ", (te_) " ", (lo) x1, " quitas)^";
      7: "No queda sitio en ", (the) x1, ".";
      8: "Hecho.";
      9: "Met", (es_) " ", (the) x1, " dentro ", (del) second, ".";
    }

  Inv:
    ! Inventario
    !  1: Mensaje si el inventario est� vac�o
    !  2: Encabezado del inventario, antes de la lista de objetos
    !     generada por la librer�a
    !  3 y 4: [6/11]
    switch (n) {
      1: "No llev", (as_) " nada.";
      2: print "Llev", (as_) "";
      3: print ":^";
      4: print ".^";
    }
  Jump:          "Salt", (as_) " en el sitio, sin ning�n resultado.";
  JumpOver, Tie: "No lograr", (as__) " nada as�.";
  Kiss:          "No creo que deb", (a_) ".";
  Listen:        "No escuch", (as_) " nada fuera de lo com�n.";
  ListMiscellany:
    ! Los siguientes mensajes se muestran como aclaraciones cuando se
    ! est� listando el inventario del jugador, o los objetos que hay
    ! en una localidad. Junto a cada mensaje se comenta qu�
    ! condiciones deben darse en ese objeto para que se muestre ese
    ! mensaje aclaratorio
    
    switch(n)
    {
     1: print " (alumbrando)";
    ! El objeto tiene "luz" y la localidad no
     2: print " (que ", (esta) x1, " cerrad", (o) x1, ")";
    ! El objeto tiene "recipiente" pero no "abierto"
     3: print " (cerrad", (o) x1, " y alumbrando)";
    ! Casos 2 y 1 juntos
     4: print " (que ", (esta) x1, " vac�", (o) x1, ")";
    ! El objeto es un recipiente abierto (o transparente) y no
    ! tiene nada en su interior
     5: print " (vac�", (o) x1, " y alumbrando)";
    ! Casos 1 y 4 juntos
     6: print " (que ", (esta) x1, " cerrad", (o) x1, " y vac�",
     (o) x1, ")";
    ! El objeto tiene "recipiente", no "abierto", pero s�
    ! "transparente"
     7: print " (cerrad", (o) x1, ", vac�", (o) x1, " y
        alumbrando)";
    ! Casos 1 y 6 juntos

! Los casos siguientes son similares, pero se muestran cuando
! se solicita a la rutina EscribirListaDesde la opcion INFOTOTAL
! La librer�a puede combinar varios de estos mensajes, por eso no
! deben llevar el cerrar par�ntesis al final, que lo a�ade la
! propia librer�a
! ************************************
!  NO SE ACONSEJA CAMBIAR ESTOS
! ************************************
    
     8: print " (alumbrando y que llevas puest", (o) x1;
    ! El objeto tiene "luz" y "puesto"
     9: print " (alumbrando";
    ! El objeto tiene "luz" pero no "puesto"
     10: print " (que llevas puest", (o) x1;
    ! El objeto tiene "puesto" pero no "luz"
     11: print " (que ", (esta) x1, " ";
    ! Mensaje introductorio para decir "que est�
    ! abierto/cerrado/vacio"...
     12: print "abiert", (o)x1;
    ! Objeto tiene "recipiente", "abrible" y "abierto" (y cosas dentro)
     13: print "abiert", (o)x1, ", pero vac�", (o) x1;
    ! Objeto tiene "recipiente", "abrible" y "abierto (pero vacio)
     14: print "cerrad", (o) x1;
    ! Objeto tiene "recipiente", "abrible"  y no "abierto"
     15: print "cerrad", (o) x1, " con llave";
    ! Objeto tiene "recipiente", "abrible" y "cerrojoechado" o "cerrojo"
     16: print " vac�", (o) x1;
    ! Objeto tiene "recipiente", no "abrible" y "transparente"
     17: print " (que ", (esta) x1, " vac�", (o) x1, ")";
    ! Como el caso anterior, pero mensaje m�s "largo" (que se activa si
    ! EscribirListaDesde es llamada sin el modo BREVE_BIT)
     18: print " que contiene ";
    ! encabezado a la lista de lo que hay dentro del objeto
     19: print " (sobre ", (el_) x1;
    ! Si el objeto tiene "soporte", la librer�a va a listar sus
    ! contenidos. Este es el encabezado de la lista
     20: print ", encima ", (del_) x1;
    ! Como e l9, pero en otra modalidad (que se activa si
    ! EscribirListaDesde es llamada sin el modo BREVE_BIT)
     21: print " (en ", (el_) x1;
    ! Si el contenido tiene "recipiente" y puede verse su interior
    ! y hay cosas, la librer�a va a mostrar sus contenidos. Este
    ! es el encabezado de la lista
     22: print ", dentro ", (del_) x1;
    ! Como el 21, pero en otra modalidad (que se activa si
    ! EscribirListaDesde es llamada sin el modo BREVE_BIT)
    }
    
  LMode1:
    ! La acci�n ModoM1 se genera ante el comando BREVE (o NORMAL). La
    ! librer�a imprime la constante Historia y a continuaci�n este
    ! mensaje.
    " est� ahora en su modo normal ~breve~, que da s�lo descripciones
    largas de los lugares la primera vez que son visitadas, y
    descripciones cortas en otro caso.";

  LMode2:
    ! La acci�n ModoM2 se genera ante el comando LARGO. La
    ! librer�a imprime la constante Historia y a continuaci�n este
    ! mensaje.
    " est� ahora en su modo ~largo~, que siempre da descripciones
    largas de los lugares (incluso si ya hab�as estado antes).";

  LMode3:
    ! La acci�n ModoM3 se genera ante el comando SUPERBREVE. La
    ! librer�a imprime la constante Historia y a continuaci�n este
    ! mensaje.
    " est� ahora en su modo ~superbreve~, que siempre da descripciones
    cortas de los lugares (incluso si nunca hab�as estado antes).";
    
   Lock:
    ! EcharCerrojo se genera con CIERRA <objeto> CON <objeto2>, o
    ! tambien ante ECHA CERROJO A <objeto>. (sin especificar un
    ! segundo objeto en este caso)
    !
    !  1: Error, el objeto que se intenta cerrar no tiene la propiedad
    !     "cerrojo".
    !  2: Error, el objeto que se intenta cerrar tiene la propiedad
    !     "cerrojo", pero ya tiene tambi�n "cerrojoechado
    !  3: Error, el objeto tiene "cerrojo", pero est� "abierto". No se
    !     le puede echar el cerrojo hasta que no se cierre.
    !  4: Error, el <objeto2> especificado no coincide con el que el
    !     <objeto> indica en su propiedad "con_llave".
    !  5: Exito, el <objeto> es cerrado (con <objeto2> si este es
    !     specificado, el cual puede encontrarse en la variable
    !     "otro")
    switch (n) {
      1: "No parece", (n) x1, " tener ning�n tipo de cerrojo.";
      2: if (x1 provides with_key)
           print_ret (The) x1, " ya estaba", (n) x1, " cerrad", (o) x1,
                     " con llave.";
         else
           print_ret (The) x1, " ya tiene",(n) x1, " echado el cerrojo.";
      3: "Primero tendr", (as__) " que cerrar ", (the) x1, ".";
      4: if (second) "No parece", (n) x1, " encajar en la cerradura.";
         "Necesit", (as_) " alg�n tipo de llave.";
      5: if (second) "", (_Cierras_) " ", (the) x1, " con ", (the) second, ".";
         "Ech", (as_) " el cerrojo ", (al) x1, ".";
    }

  Look:
    ! La acci�n Look se genera cuando el player pone MIRAR, pero
    ! tambi�n de forma autom�tica al entrar en una localidad nueva, o
    ! cuando el player sale/se baja de un objeto en el que estaba.
    !
    ! Algnouns de los mensajes que se definen aqui aparecen en el
    ! "t�tulo" de la localidad (lo que aparece en negrita before de la
    ! descripci�n de la localidad)
    !
    !  1: aclaraci�n a a�adir tras el t�tulo si el player est� subido
    !     en un objeto
    !  2: aclaraci�n a a�adir tras el t�tulo si el player est� dentro
    !     de un objeto
    !  3: aclaraci�n a a�adir tras el t�tulo si el player ha sido
    !     transformado en otro personaje, mediante una llamada a
    !     ChangePlayer(nuevo, 1); (si el 1, la librer�a no intentar�
    !     mostrar este mensaje)
    !  4: Una vez que la librer�a ha mostrado la descrcipci�n de la
    !     localidad, si el player estaba encima de un supporter, la
    !     librer�a generar� este mensaje 4 para mostrar qu� m�s cosas
    !     hay sobre el supporter
    !  Restante: 5,6 [6/11] antes 'default'
    !     Son usados por la librer�a para listar los objetos "simples"
    !     que hay en la localidad. Son objetos "simples" los que no
    !     dan su propia auto-descrcipcion a trav�s de la propiedad
    !     "describe" o "initial".
    !  7: Respuesta estandar para MirarHacia [6/11]
    switch (n) {
      1:   print " (sobre ", (the) x1, ")";
      2:   print " (en ", (the) x1, ")";
      3:   print " (como ", (object) x1,")";
      4:   print "^Sobre ", (the) x1; ! ELIUK: agregado salto de linea inicial (solo I6)
           WriteListFrom(child(x1),
                         ENGLISH_BIT + RECURSE_BIT + PARTINV_BIT
                         + TERSE_BIT + ISARE_BIT + CONCEAL_BIT);
           ".";
     5, 6: new_line; ! ELIUK: agregado salto de linea inicial (solo I6)
           if (x1 ~= location) {
             if (x1 has supporter) print "Sobre "; else print "En ";
             print (the) x1;
             print " ", (puedes_) " ver ";
           } else print "", (_Puedes_) " ver ";

           if (n == 5) print "tambi�n ";
           WriteListFrom(child(x1),
                         ENGLISH_BIT + WORKFLAG_BIT + RECURSE_BIT
                         + PARTINV_BIT + TERSE_BIT + CONCEAL_BIT);
           if (x1 ~= location) ".";
           ".";
      7:   "No observ", (as_) " nada digno de menci�n al mirar hacia ", (the) x1, ".";![6/11] ! ELIUK: sobraba el "m�s"
      8:   if (x1 has supporter) print " (sobre "; else print " (en "; ! added in 6/11N 4U65
           print (the) x1, ")";
    }

  LookUnder:
    ! Dos casos:
    !  1: Error, estamos a oscuras
    !  2: Exito, mensaje por defecto.
    switch (n) {
      1: "Est� muy oscuro.";
      2: "No v", (eo_) " nada interesante.";
    }

  Mild:  "Bastante.";
  ! Y esta si usa como primera palabra alg�n insulto m�s suave
  
!================================================================
! MISCELANEA
!================================================================
! Esta es una falsa acci�n, que se usa simplemente para agrupar aqu�
! todos los mensajes de error del parser, los mensajes ante algunos
! metacommandos, o algnouns mensajes de librer�a no relacionados con
! acci�n alguna. La asignaci�n de n�meros a mensajes es bastante
! caprichosa.

! Debajo de cada mensaje un comentario indica en qu� condiciones se
! genera.

 Miscellany:
    switch (n) {
      1: "(S�lo considero los diecis�is primeros objetos)^";
    ! El player ha especificado objetos multiples, pero el parser
    ! tiene una lista que no admite m�s de 16, de modo que los
    ! restbefore objetos no ser�n procesados.

      2: "�No s� a qu� objetos te refieres!";
    ! El player ha usado objetos multiples, pero por alguna raz�n
    ! la lista del parser est� vac�a.

      3: 
!        TextoLlamativo(" Has muerto ");
         print " Has muerto ";
    ! Mensaje fin de juego. Has muerto.

      4: 
!        TextoLlamativo(" Has ganado ");
         print " Has ganado ";
    ! Mensaje fin de juego de victoria.

      5: ! Mensaje a mostrar tras el fin del juego, para pedir al
         ! player si quiere reniciar, recuperar, anular, puntuaci�n,
         ! curiosidades o terminar.
         print "^�Quieres REINICIAR, RECUPERAR un juego guardado";
         #IFDEF DEATH_MENTION_UNDO;
           print ", ANULAR tu �ltimo movimiento";
         #ENDIF;
!        #IFNDEF NO_PUNTUACION;
         if (TASKS_PROVIDED == 0)
           print ", ver la PUNTUACI�N de este juego";
!        #ENDIF; ! NO_PUNTUACION
    
         if (deadflag == 2 && AMUSING_PROVIDED == 0)
           print ", ver algunas CURIOSIDADES";

         " o TERMINAR?";

      6: "[Tu int�rprete no puede ~deshacer~ acciones, �lo siento!]";
         ! Error si el int�rprete no tiene "undo"
    #Ifdef TARGET_ZCODE; ![6/11]
      7: "~deshacer~ fall�. [No todos los int�rpretes lo tienen.]";
    #Ifnot; ! TARGET_GLULX
      7:  "[No puedes ~deshacer~ m�s.]";
    #Endif; ! TARGET_
     ! Otro error si el int�rprete no tiene "undo", pero no hab�a
     ! sido detectado correctamente

      8: "Por favor, da una de las respuestas anteriores.";
    ! Si el player no responde correctamente a la pregunta del
    ! mensaje [Miscellany,5]

      9: "^�", (_Te_) " h", (e_) " quedado a oscuras!"; ! ELIUK: agregado salto de linea inicial (solo I6)
    ! Mensaje cuando se detecta, al final del turno, que ya no hay
    ! light.

     10: "�Perd�n?";
    ! Mensaje cuando el player introduce una linea en blanco (no
    ! llega a generarse acci�n alguna, por tanto no se consume
    ! turno ni se ejecutan daemons)

     11: "[No ", (puedes_) " ~deshacer~ lo que no h", (e_) " hecho.]";
    ! Si el player intenta "undo" al principio del juego

     12: "[No ", (puedes_) " ~deshacer~ dos veces seguidas. �Lo siento!]";
    ! Si el player pone "undo" dos veces

     13: "[Retrocediendo al turno anterior.]";
    ! Exito en el "undo"

     14: "Lo siento, eso no puede corregirse.";
    ! Si el player pone EEPA (verbo para corregir), pero el
    ! parser no recordaba ninguna palabra mal
    ! El commando EEPA se usa as�, por ejemplo:
    !   > SACA MAZANA VERDE DE LA CESTA
    !   No veo eso por aqu�
    !   > EEPA MANZANA
    !   Sacas la manzana verde de la cesta de mimbre.
    
     15: "No te preocupes.";
    ! Si el player pone EEPA s�lo (sin especificar la palabra
    ! corregida)

     16: "~eepa~ s�lo puede corregir una palabra.";
    ! Si el player pone EEPA seguido de dos o m�s palabras

     17: "Est� muy oscuro y no ", (puedes_) " ver nada.";
    ! Descripci�n del objeto Oscuridad

     18: print "", (ti_) " mism", (os_) "";
    ! Nombre corto del objeto player

     19: "Tan buen aspecto como siempre.";
    ! Descripci�n del objeto player

     20: "Para repetir un comando como ~rana, salta~, escribe
    ~repite~, en lugar de ~rana, repite~.";
    ! Cuando el player pone PNJ, REPITE para que el PNJ haga otra
    ! vez la �ltima acci�n.

     21: "No hay comando que repetir.";
    ! Cuando el player pone REPITE como primer commando del juego.

     22: "No puedes empezar la frase con una coma.";
    ! Cuando ha empezado la frase con una coma :-)

     23: "Parece que quieres hablar con alguien, pero no veo con
    qui�n.";
    ! Cuando el player pone PEPE, COGE HACHA, pero no hay un
    ! objeto que responda al name PEPE.

     24: "No ", (puedes_) " hablar con ", (the) x1, ".";
    ! Si el player intenta hablar con un objeto que no es
    ! "animate" ni "talkable" ("talkable" debe ponerse a objetos
    ! inanimates con los que se podr�a hablar, como un micr�fono)

     25: "Para hablar con alguien intenta ~alguien, hola~ o algo
         as�.";
    ! Si hay palabras no comprendidas entre el name del PNJ y
    ! la comma, como p.ej: PEPE XADASDGG, SALTA

     26: print "(primero trat", (as_) " de ", (coge) "r ";
         if (x1 has animate) print (al) x1;
         else                print (the) x1;
         ")"; ! infsp I6

    ! Si la gram�tica especifica que una acci�n debe efectuarse
    ! sobre un objeto en poder del player (token "held"), pero el
    ! player lo intenta sobre un objeto que no tiene, pero que
    ! est� accesible, el parser intentar� autom�ticamente coger
    ! ese objeto antes de efectuar la acci�n. En este mensaje se
    ! avisa al player de esta intenci�n.

! ================================================================    
! ERRORES DE PARSING
! Estos errores son generados por la librer�a, a menos que el
! programador proporcione su propia funci�n ParserError para
! manejo de errores.

     27:! El parser se ha atascado. Es el tipo de error m�s
    ! gen�rico. Si el parser puede, dar� un error m�s concreto
    ! (noun de los siguientes)
    "No entend� esa frase.";

     28:! Al parser le sobran palabras, aunque ha entendido gran parte
    ! de la orden. 
        
    print "Intenta de nuevo, porque s�lo te pude entender: ";

     29: ! El parser esperaba un token de tipo n�mero
    "No comprend� ese n�mero.";

     30: ! El parser no ha comprendido el nombre del objeto al que se
    ! refiere el player, o ese objeto no est� aqu�.
    "No v", (eo_) " eso que dices.";

     31: ! Este error parece que no se genera nunca. Un posible bug de
    ! libreria, o restos de versiones arcaicas
     "�Pareces haber dicho muy poca cosa!";

     32: ! El objeto que el player intenta usar no est� en su
    ! poder. El parser ha intentado "cogerlo de forma automatica",
    ! pero ha fallado
    "�No ", (tienes_) " eso!";

     33: ! El player ha intentado usar objetos m�ltiples (o "TODO")
    ! con un verbo que en su gram�tica no lo admite.
    "No puedes especificar objetos m�ltiples con ese verbo.";

     34: ! El player ha intentado objetos multiples para "noun" y para
    ! "second", como en METE TODO EN TODO.
     "S�lo puedes especificar objetos m�ltiples una vez en cada
    l�nea.";

     35: ! El player ha usado un proname, como COMELO, pero el
    ! parser no tiene asignado un valor a ese proname.
     "No estoy seguro de a qu� se refiere ~", (address) pronoun_word,
     "~.";

     36: ! El player ha puesto una lista de objetos y ha usado la
    ! palabra EXCEPTO para exceptuar algo que no estaba incluido
    ! en la lista, por ejemplo: COGE TODAS LAS MONEDAS EXCEPTO EL
    ! SACO.
    "Has exceptuado algo que no estaba incluido.";

     37:! La gram�tica de un verbo obliga a que el objeto sobre el que
    ! se act�e sea animate (tiene un token de tipo "creature"),
    ! pero el player lo ha intentado sobre un objeto que no es
    ! animate.
    "S�lo ", (puedes_) " hacer eso con seres animados.";

     38: ! La primera palabra usada por el player es desconocida (o
    ! la primera palabra tras la comma, si se trata de una orden a
    ! un PNJ)
    "No conozco ese verbo.";

     39: ! El player intenta actiones sobre las palabras que est�n en
    ! la propiedad "irrelevante" de la localidad
     "Eso no es importante.";

     40: ! El player ha usado un proname, como EXAMINALO, pero el
    ! proname se refiere a un objeto que ya no est� visible.
     "Ahora mismo no ", (puedes_) " ver lo que representa el pronombre ~",
      (address) pronoun_word, "~ (", (the) pronoun_obj, ").";

     41: ! Este error no sale nunca. Probablemente sigue aqui por
    ! razones de compatibilidad, o por despiste de Graham �Tal vez
    ! ha sido sustituido en versiones mas recients por el mensaje
    ! n�mero 28?
    "No entend� la �ltima parte de la orden.";

     42: ! El player ha solicitado un numero de objetos en una lista
    ! de objetos multiples, pero el parser no es capaz de
    ! encontrar tantos. Por ejemplo: COGE SEIS MONEDAS.
    ! En x1 se recibe el n�mero de objetos hallados por el
    ! parser.
     if (x1==0) "No hay suficientes.";
         else if (x1==1){
     print "Aqu� s�lo hay un";
     if (multiple_object-->1) print (o) multiple_object-->1;
     else print "o";
     " disponible.";
        }
    else
       "S�lo hay ", (number) x1, " disponibles para esa acci�n.";

     43: ! El player ha puesto TODO como objeto m�ltiple, pero el
    ! parser no ha encontrado ning�n objeto. En realidad este
    ! error parece no producirse nunca (second bug de librer�a) ya
    ! que el que siempre acaba gener�ndose es el siguiente (44)
     "�No ", (encuentro_) " nada para hacer eso!";

     44: ! El player ha intentado objetos m�ltiples usando TODO
         ! pero el parser no ha encontrado ninguno. Por ejemplo, COGE
         ! TODO o DEJA TODO si no hay nada cerca o en el inventario.
         ! Modificado en [020621] para dar un mensaje m�s coherente
         ! si el player tiene alg�n objeto en su inventario.
       if (action_to_be == ##Drop) {
         print "�Pero si no llev", (as_) " nada";
         if (children (player))
           " de eso!";
         else
           "!";
       }
       print "No hay nada para ";
       LanguageVerb(verb_word);
       ".";

     45:! El player ha nombrado un PNJ ambiguo (hay m�s de noun con el
    ! mismo name cerca). El parser le pide aclaraci�n. Este
    ! mensaje es la primera parte de la pregunta, el resto lo pone
    ! el parser (que es la lista de PNJs entre los que
    ! duda). Debemos poner a 1 la variable PreguntaCualExactamente
    ! cuando se generan este tipo de preguntas aclaratorias.
     print "�Qui�n concretamente, ";
    PreguntaCualExactamente=1;
    ImprimirListaDudosos("o");
!    print "?^";

     46:! El player ha nombrado un objeto ambiguo (hay m�s de noun con el
    ! mismo name cerca). El parser le pide aclaraci�n. Este
    ! mensaje es la primera parte de la pregunta, el resto lo pone
    ! el parser (que es la lista de objetos entre los que
    ! duda). Debemos poner a 1 la variable PreguntaCualExactamente
    ! cuando se generan este tipo de preguntas aclaratorias.
     print "�Cu�l concretamente, ";
    PreguntaCualExactamente=1;
    ImprimirListaDudosos("o");
!    print "?^";

     47: ! El player ha respondido "TODOS" o "AMBOS" a una pregunta
    ! aclaratoria como la anterior, pero el verbo no admite
    ! objetos m�ltiples
    print "Lo siento, s�lo puedes referirte a un objeto aqu�. �Cu�l
        exactamente?^";
    PreguntaCualExactamente=1;

     48: !El player ha escrito una frase u orden a PSI incompleta,
         !como BESA (verbo aplicado sobre PSIs normalmente).
         !El parser le pregunta para que complete la frase
     PreguntaCualExactamente=1;
!     print "Mensaje 48^"; ![infps] debug
    if (IniciarPregunta()==0) print "A qui�n";
    if (actor ~= player) print " tiene que ";
    else print " quieres ";
         IdiomaImprimirComando();
    if (actor~=player) print " ", (the) actor;
    print "?^";

     49: !El player ha escrito una frase u orden incompleta,
         !como COGE (verbo aplicado sobre objetos normalmente).
         !El parser le pregunta para que complete la frase
    PreguntaCualExactamente=1;
!     print "Mensaje 49^"; ![infps] debug
    if (IniciarPregunta()==0) print "Qu�";
    if (actor ~= player) print " tiene que ";
    else print " quieres ";
         IdiomaImprimirComando();
    if (actor~=player) print " ", (the) actor;
    print "?^";

!#IFNDEF NO_PUNTUACION;
     50: ! El player acaba de realizar una acci�n que punt�a (la
    ! librer�a ha detectado que la variable score ha cambiado
    ! de valor). Se informa de ello al player. El par�metro x1
    ! contiene el incremento (o decremento) en la puntuaci�n.
    print "Tu puntuaci�n ha ";
    if (x1 > 0)
          print "aumentado";
        else {
          x1 = -x1;
          print "disminuido";
        }
    print " en ", (number) x1, " punto";
    if (x1 > 1) print "s";
!#ENDIF; ! NO_PUNTUACION

     51: ! El player ha intentado una acci�n con objeto m�ltiple
    ! (como COGE TODO). Esta acci�n es convertida por el parser en
    ! una serie de actiones COGE OBJETO1, COGE OBJETO2, etc... Si
    ! en la ejecuci�n de alguna de estas actiones encuentra que la
    ! localidad del player ha cambiado, debe abortar el
    ! proceso. Este mensaje informa de ello al player.
    !
    "(Ya que ha ocurrido algo dram�tico, se ha recortado la lista
    de objetos sobre los que actuabas)";
    
     52: ! En los men�s, se espera a que el player escriba un n�mero
    ! para mostrar una entrada del men�. Si pulsa un numero
    ! incorrecto, se saca este mensaje. El par�metro x1 contiene
    ! el n�mero m�s alto v�lido
    "^Escribe un n�mero del 1 al ", x1,
     ", 0 para volver a mostrarlo, o pulsa Intro.";
    
     53: ! Mensaje que muestra el sistema de men�s cuando espera una
    ! tecla
     "^[Por favor, pulsa ESPACIO]";
     ! [6/11] agregados:
     54: "[Comentario guardado]";
     55: "[Comentario NO guardado]";
     56: print ".^";
     57: print "?^";
    }        
    
  No, Yes: "S�lo era una pregunta ret�rica.";
  ! Cuando el jugador pone SI o NO, a secas, se genera esta
  ! acci�n. Est� pensado por si el juego suelta una frase del estilo
  ! de "�Pero t� eres tonto?" y el jugador responde NO.
  ! De todas formas, hay un problema, y es que NO es normalmente la
  ! acci�n Ir al noroeste. Si queremos que NO sea provisionalmente
  ! la acci�n No, entonces el juego debe poner un 1 en la variable
  ! PreguntaSiNo, antes de escribir el texto "�Pero t� eres tonto?".
  ! En este caso, el parser interpretar� "NO" como la acci�n No,
  ! pero s�lo en el siguiente turno. Despu�s recuperar� su antiguo
  ! significado de Ir al Noroeste.

  NotifyOff:
    "Notificaci�n de puntuaci�n desactivada.";
    
  Objects:
    ! Ante el verbo "OBJETOS" se genera esta acci�n, que muestra una
    ! lista de todos los objetos que el jugador ha manipulado a lo
    ! largo del juego, junto con una indicaci�n de qu� hizo con ellos.
    !
    !  1: Texto introductorio a la lista
    !  2: Si no ha manejado a�n ninguno
    !  3: indicaci�n de que lo lleva puesto
    !  4: indicaci�n de que lo lleva en su inventario
    !  5: indicaci�n de qui�n lo tiene ahora (si lo tiene un PNJ)
    !  6: indicaci�n de d�nde est� el objeto (si est� en una localidad)
    !  7: indicaci�n de d�nde est� el objeto (si est� en un "entrable")

    !  8: indicaci�n de d�nde est� el objeto (si est� en un recipiente)
    !  9: indicaci�n de d�nde est� el objeto (si est� en un soporte)
    !  10: indicaci�n de que se ha perdido (no est� en ninguno de los
    !     casos anteriores)
    switch(n)
    {
     1: "Objetos que has manejado:^";
     2: "Ninguno.";
!    3: print "   (puest", (o) noun, ")";
     3: print "   (puesto)"; ! [infsp] fixed, no se pasa el objeto
!   4: print "   (l", (o) noun, " llevas)";
     4: print "   (contigo)"; ! [infsp] fixed
!     5: print "   (en poder ", (del) noun, ")"; !siendo x1 el animate que lo tiene
     5: print "   (dejado con alguien)";! [infsp] fixed
     6: print "   (ahora en ", (name) x1, ")";
     7: print "   (ahora en ", (the) x1, ")";
     8: print "   (ahora dentro ", (del) x1, ")";
     9: print "   (ahora en ", (the) x1, ")";
!     10: print "   (perdid", (o) x1, ")";
      10: print "   (perdido)"; ! [infsp] fixed
}

  Open:
    !  1: Error, el objeto no tiene el atributo "abrible"
    !  2: Error, el objeto es abrible, pero tiene "cerrojoechado"
    !  3: Error, el objeto es abrible, pero ya tiene el atributo
    !     "abierto"
    !  4: Exito, el objeto se abre, y adem�s era recipiente y ten�a
    !     cosas dentro. Se muestra entoces la lista de objetos que
    !     hab�a en su interior (si habia algo)
    !  5: Exito, el objeto se abre (pero no tiene nada en su
    !     interior, o no era recipiente).
    switch (n) {
      1: print_ret "No es algo que pueda abrirse.";
      2: "Est�", (n) x1, " cerrad", (o) x1, " con llave.";
      3: "Ya estaba", (n) x1, " abiert", (o) x1, ".";
      4: print "Abr", (_es_) " ", (the) x1, ", descubriendo ";
         if (WriteListFrom(child(x1),
                           ENGLISH_BIT + TERSE_BIT + CONCEAL_BIT)==0) "nada.";
         ".";
      5: "Abr", (_es_) " ", (the) x1, ".";
    }

  Order:
    print_ret (The) x1, " tiene", (n) x1, " mejores cosas que hacer.";

  Places: 
    ! El verbo "LUGARES" muestra un listado de los lugares que el
    ! jugador ha visitado. Aqu� debemos escribir el texto
    ! introductorio a esa lista.
    switch (n) {
        1:  print "Has visitado: ";
        2:  print ".^";
    }

  Pray: print "No ob", (tienes_) " nada pr�ctico de ";
        if (player provides persona) {
           if (player.persona & TERCERA_PERSONA) {
             if (player.persona & PERSONA_PLURAL) print "su";
             else                                 print "su";
           } else if (player.persona & PRIMERA_PERSONA) {
             if (player.persona & PERSONA_PLURAL) print "nuestra";
             else                                 print "mi";
           } else { ! Segunda persona
             if (player.persona & PERSONA_PLURAL) print "vuestra";
             else                                 print "tu";
           }
         } else { ! Por defecto, segunda persona del singular
           print "tu";
         }        
        "s oraciones.^";

  Prompt:
    print "^>";
    ! El prompt aparece justo antes de pedir una nueva l�nea al
    ! jugador. Puede definirse para que sea una frase, o se genere una
    ! frase aleatoria en cada turno.
    
  Pronouns:
    ! Esta acci�n (generada por el verbo PRONOMBRES) muestra una lista
    ! de qu� significa en ese momento para el parser cada proname
    ! -lo, -la, -los, -las...
    !
    !  1: Texto introductorio
    !  2: La palabra "es"
    !  3: texto a mostrar si el proname no est� definido
    !  4: Si el juego no define pronames
    !  5: [6/11]
    switch(n)
    {
     1: print "En este momento, ";
     2: print "es ";
     3: print "no est� definido";
     4: "el juego no conoce ning�n pronombre.";
     5:  ".";
    }
    
  Pull, Push, Turn:
  ! Las acciones Tirar, Empujar y Girar generan todas ellas los
  ! mismos mensajes, pero pueden darse 4 casos:
  !   1: Que el objeto tenga el atributo 'estatico'
  !   2: Que el objeto tenga el atributo 'escenario'
  !   3: El objeto no tiene 'estatico' ni 'escenario'
  !   4: El objeto es un ser animado
  ! [Nota, si el objeto tiene ambos 'estatico' y 'escenario' se
  ! est� tambi�n en el caso 1]
    switch (n) {
      1: "Est�", (n) x1, " firmemente sujet", (o) x1, ".";
      2: "No ", (eres_ ) " capa", (z_) ".";
      3: "No ocurre nada, aparentemente.";
      4: "Eso ser�a, como poco, maleducado.";
    }

! Push: see Pull    

  PushDir:
  ! La acci�n EmpujarDir se genera ante frases como EMPUJA EL COCHE
  ! HACIA EL NORTE. Hay 3 casos posibles:
  !   1: Si el juego no ha previsto la posibilidad de empujar ese
  !      objeto hacia una direcci�n (o sea, no llama a la rutina
  !      PermitirEmujarDir). Esta es la respuesta por defecto m�s
  !      habitual.
  !   2: Si el jugador pone tras HACIA algo que no es una direcci�n.
  !      (respuesta por defecto en este caso)
  !   3: Si intenta empujar hacia arriba o hacia abajo
    switch(n) {
      1: print "No creo que empujar ";
         if (x1 has animate) print (al) x1;
         else                print (el) x1;
         " sirva para nada.";
      2: "Eso no es una direcci�n.";
      3: "No, no ", (puedes_) " en esa direcci�n.";
    }
   
  PutOn:
    ! 1: Error, el objeto no est� en poder del jugador. [Nota,
    !    conviene mirar en este caso si el objeto es animado o no,
    !    para generar un mensaje m�s adecuado]
    ! 2: Error, el jugador intenta poner un objeto sobre s� mismo
    ! 3: Error, el jugador intenta poner el objeto sobre otro que no
    !    tiene el atributo "soporte"
    ! 4: Error, el jugador intenta poner un objeto sobre el propio
    !    jugador.
    ! 5: Aviso, el objeto es una prenda puesta. La librer�a va a
    !    quit�rselo autom�ticamente antes de reintentar la acci�n
    !    PonerSobre
    ! 6: Error, se intenta poner el objeto sobre otro en el que ya no
    !    queda sitio (el n�mero de objetos que caben en el soporte se
    !    indica en su propiedad "capacidad")
    ! 7: Exito. Mensaje a mostrar para cada objeto puesto cuando se
    !    ponen muchos (ej: PON TODO SOBRE LA MESA)
    ! 8: Exito. Mensaje a mostrar cuando se pone un solo objeto sobre
    !    otro.
    switch (n) {
      1: if (x1 has animate)
           "Antes tendr", (ias_) " que", (coge) "rl", (o) x1, ", y no ",
           (_se_) " si se dejar�", (n) x1, ".";
          else
            "Necesit", (as_) " tener ", (the) x1, " para poder poner",
            (lo) x1, " donde sea.";
      2: "No ", (puedes_) " poner un objeto sobre s� mismo.";
      3: "Poner cosas sobre ", (the) x1, " no servir� de nada.";
      4: "", (_Te_) " falta destreza.";
      5: "(primero ", (se_) " ", (lo) x1, " quit", (as_) ")^";
      6: "No queda sitio en ", (the) x1, " para poner nada m�s.";
      7: "Hecho.";
      8: "Coloc", (as_) " ", (the) x1, " sobre ", (the) second, ".";
    }
   
  Quit:
    !  1: Respuesta al comando FIN
    !  2: Respuesta si el jugador escribe algo distinto de "si" o "no"
    switch (n) {
      1: print "Por favor, responde s� o no.";
      2: print "�Realmente quieres abandonar el juego? ";  ! ELIUK BLAU: antes "Seguro que..."
    }
    
  Remove:
    ! 2 mensajes de error y 1 de exito
    !   1: El recipiente que contiene el objeto que el jugador quiere
    !      sacar, est� cerrado. (ml_o apunta al objeto, no al
    !      recipiente, el cual puede obtenerse con parent(ml_o))
    !   2: El objeto no est� dentro del recipiente que el jugador ha
    !     dicho.
    !   3: Exito
    switch (n) {
      1: "Por desgracia ", (the) parent(x1), " ", (esta) parent(x1),
         " cerrad", (o) parent(x1), ".";
      2: if (second has animate)
           "�Pero si no ", (lo) x1, " tiene", (n) second, "!";
         "�Pero si no ", (esta) x1, " ah� ahora!";
      3: if (verb_word == 'quita') "Quitad", (o) x1, ".";
         "Sacad", (o) x1, ".";
    }

  Restart:
    !  1: Respuesta al comando REINICIAR
    !  2: Si no se puede reiniciar
    switch (n) {
      1: print "�Realmente quieres reiniciar el juego? ";  ! ELIUK BLAU: antes "Seguro que..."
      2: "Error.";
    }

  Restore:
    ! 1: Si no se pudo cargar la partida con LOAD
    ! 2: Exito (no llega a mostrarse, porque en caso de �xito el
    !    estado del juego cambia al momento en que se salv�. El "Ok"
    !    que se ve entonces es el del comando Salvar.
    switch (n) {
      1: "Error. No se pudo recuperar la partida.";
      2: "Ok.";
    }

  Rub: "Ya ", (esta) x1, " bastante limpi", (o) x1, ".";

  Save:
    ! 1: Si no se pudo guardar el juego
    ! 2: �xito
    switch (n) {
      1: "Error. No se pudo guardar.";
      2: "Ok.";
    }

    ! Puntuaci�n del juego. Puede activarse porque el player lo pide
    ! (con el verbo "PUNTUACION") o porque el juego ha terminado. En
    ! este ultimo caso, la variable "deadflag" valdr� true.
    ! [6/11] agregado de (2)
  Score:
    switch (n) {
      1: if (deadflag) print "En este juego, ", (tu_) " puntuaci�n ha sido ";
         else          print "Hasta el momento tu puntuaci�n es ";
         print score, " de un total de ", MAX_SCORE, ", en ", turns, " turno";
         if (turns>1) print "s"; return;
      2: "Este juego no tiene conteo de puntuaci�n.";
    }

  ScriptOff:
    ! 1: Error ya estaba desactivada la transcrcipci�n
    ! 2: Exito
    switch (n) {
      1: "La transcripci�n ya estaba desactivada.";
      2: "^Fin de la transcripci�n.";
      3: "Intento fallido de finalizaci�n de transcripci�n.";
    }

  ScriptOn:
    ! 1: Error, ya estaba activada la transcrcipci�n
    ! 2: �xito (parte inicial del mensaje, la librer�a a�ade el t�tulo
    !    del juego obtenido de la constante Historia)
    switch (n) {
      1: "La transcripci�n ya estaba activada.";
      2: "Iniciando la transcripci�n de ";
      3: "Intento fallido de comienzo de transcripci�n.";
    }

  Search:
    ! La acci�n BuscarEn se genera ante la frase MIRA EN RECIPIENTE,
    ! MIRA EN SOPORTE, REGISTRA OBJETO.
    !
    !  1: Error, estamos a oscuras.
    !  2: �xito examinando soporte, pero no hay nada sobre �l.
    !  3: �xito examinando soporte, listar lo que hay.
    !  4: El objeto examinado no es soporte ni recipiente, mensaje
    !     est�ndar indicando que no se encuentra nada.
    !  5: Error, El objeto examinado es recipiente, pero est� cerrado.
    !  6: �xito, el objeto examinado es recipiente abierto, pero no
    !     tiene nada dentro.
    !  7: �xito, el objeto examinado es recipiente abierto y tiene
    !     cosas dentro. Listarlas.
    switch (n) {
      1: "Est� muy oscuro.";
      2: "No hay nada sobre ", (the) x1, ".";
      3: print "Sobre ", (the) x1;
         WriteListFrom(child(x1),
                       TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
         ".";
      4: "No ", (encuentro_) " nada interesante.";
      5: "No ", (puedes_) " ver lo que hay dentro ", (del) x1, " porque ",
         (esta) x1, " cerrad", (o)x1, ".";
      6: print_ret (The) x1, " ", (esta) x1, " vac�", (o) x1,".";
      7: print "En ", (the) x1;
         WriteListFrom(child(x1),
                       TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
         ".";
    }

  Set: "No, no ", (puedes_) ".";
  ! Fijar sale ante la frase FIJA OBJETO o AJUSTA OBJETO

  SetTo: "Eso no puede regularse a ning�n valor.";
  ! PonerA sale ante la frase AJUSTA DIAL A 23 o FIJA CONTROL EN
  ! ACTIVADO, o PON REGULADOR A 30
  
  Show:
    ! 1: Error, el jugador intenta mostrar un objeto que no tiene.
    ! 2: Aviso, se ha enviado la acci�n Mostrar al PNJ, pero este no
    !    ha respondido. En este caso ml_n apunta al PNJ y no al objeto
    !    que le hemos mostrado.
    switch (n) {
     1: "No ", (tienes_) " ", (the) x1, ".";
     2: print_ret (The) x1, " no muestra", (n) x1, " inter�s.";
    }
    
  Sing:  "Cant", (as_) " fatal.";
  Sleep: "No est", (oy_) " especialmente somnolient", (o) player, ".";
  Smell: "No ", (hueles_) " nada extra�o.";
  Sorry: "Oh, no es necesario que te disculpes.";
  ! Esta acci�n se genera si el jugador pone "LO SIENTO",
  ! "PERDONA" o cosas similares.

  Squeeze:
    ! Dos casos:
    !  1: Tratar de retorcer un ser animado
    !  2: Cualquier otro objeto
    switch (n) {
      1: "�Las manos quietas!";
      2: "No consig", (ues_) " nada haciendo eso.";
    }

  Strong: "Los verdaderos aventureros no usan ese vocabulario.";
  ! Y esta si usa como primera palabra (verbo) un taco.
  Swim:   "No hay agua suficiente en la que nadar.";
  Swing:  "No es adecuad", (o) x1, " para columpiarse.";

  SwitchOff:
    !  1: Error, el objeto no tiene el atributo "conmutable"
    !  2: Error, el objeto ya tenia desactivado el atributo
    !    "encendido"
    !  3: Exito, el objeto tiene ahora desactivado "encendido"
    switch(n) {
      1: print_ret "No es algo que pueda apagarse.";
      2: print_ret "Ya estaba", (n) x1, " apagad", (o) x1, ".";
      3: "Apag", (as_) " ", (the) x1, ".";
    }

  SwitchOn:
    ! Generada al intentar ENCIENDE OBJETO si el objeto tiene el
    ! atributo "conmutable". Observar que si no lo tiene, la acci�n
    ! que se genera ser� sin embargo Quemar.
    !
    !   1: Error, el objeto no es conmutable (este error no aparecer�
    !      si el jugador pone ENCIENDE OBJETO, ya que en este caso la
    !      acci�n generada ser� Quemar objeto, pero puede aparecer si
    !      pone CONECTA OBJETO)
    !   2: Error, el objeto ya ten�a el atributo "encendido"
    !   3: Exito, el objeto tiene ahora activado "encendido".
    switch (n) {
      1: print_ret "No es algo que pueda encenderse.";
      2: print_ret "Ya estaba", (n) x1, " encendid", (o) x1, ".";
      3: "Enciend", (es_) " ", (the) x1, ".";
    }
    
  Take:
    ! Hay 13 casos diferentes. Se trata de 1 mensaje de �xito y 11
    ! mensajes de error y un aviso
    !  1: Exito
    !  2: Error, el objeto cogido es el propio jugador
    !  3: Error, el objeto cogido es animado
    !  4: Error, el jugador est� subido o metido en el objeto que
    !     intenta coger
    !  5: Error, el jugador ya ten�a ese objeto
    !  6: Error, el objeto est� en poder de un ser animado [ml_o
    !     apunta al ser, no al objeto, el cual puede obtenerse en la
    !     variable "uno"]
    !  7: Error, el objeto es un sub-objeto de otro (est� dentro de un
    !     objeto que tiene el atributo "transparente", pero no tiene el
    !     atributo "recipiente" ni "soporte")
    !  8: Error, el objeto est� dentro de otro, pero este otro no es
    !     recipiente ni soporte ni transparente (probablemente un error
    !     de programaci�n)
    !  9: Error, el objeto est� dentro de un recipiente cerrado.
    ! 10: Error, el objeto es escenario
    ! 11: Error, el objeto es estatico (no puede ser llevado de la
    !     localidad en que se halla)
    ! 12: Error, el jugador lleva demasiados objetos
    ! 13: Mensaje de aviso, la librer�a ha decidido mover objetos del
    !     inventario al "objeto saco" para hacer sitio y que el jugador
    !     pueda coger el objeto. [El objeto saco es uno que puede
    !     declarar el programador. Si el jugador lo coge y lo lleva
    !     consigo, la librer�a lo usar� autom�ticamente como mochila
    !     para descargar al jugador cuando sea necesario]
    switch (n) {
!      1: "Hecho."; ! [infsp] fixed, no se pasa el objeto
       1: if (dialecto_sudamericano) print "Tomad";
          else                       print "Cogid";
          print_ret (o) noun, ".";
       2: "Siempre ", (se_) " ", (tienes_) " a ", (ti_) " mism", (o) player, ".";
       3: "No creo que ", (al) x1, " le", (s) x1, " gustara.";
       4: print "", (_Tienes_) " que ";
          if (x1 has supporter) print "bajar", (se_) " "; else print "salir", (se_) " ";
          print_ret (del) x1, " antes.";
       5: "Ya ", (tienes_) " ", (the) x1, ".";
       6: "Parece que pertenece", (n) noun, " ", (al) x1, ".";
       7: "Parece", (n) noun, " formar parte ", (del) x1, ".";
       8: "No ", (esta) x1, " disponible", (s) x1, ".";
       9: print_ret (The) x1, " no ", (esta) x1, " abiert", (o) x1, ".";
      10: "Dif�cilmente podr", (ias_) " llevar", (se_) " eso.";
      11: print_ret (The) x1, " est� fij", (o) x1, " en el sitio.";
      12: "Ya llev", (as_) " demasiadas cosas.";
      13: "(coloc", (as_) " ", (the) x1, " en ", (the) SACK_OBJECT,
          " para hacer sitio)";
    }

  Taste: "No sabore", (as_) " nada inesperado.";
  
  Tell:
    ! Dos casos:
    !  1: El jugador habla consigo mismo
    !  2: Hablar con cualquier otro ser animado
    ! [Nota: Hablar con un objeto inanimado no est� permitido por el
    ! parser, que da un error y no llega a generar la acci�n]
    ! [Los errores de parser se tratan en la secci�n "Miscelanea" de
    ! esta rutina, al final]
    switch (n) {
      1: "Habl", (as_) " sol", (o) player, " durante un rato.";
      2: "No h", (e_) " provocado ninguna reacci�n.";
    }
    
  Think:   "Vaya. Qu� buena idea.";
  ThrowAt:
    ! Dos casos:
    !  1: Lanzar cosas a objetos inanimados
    !  2: Lanzar cosas a objetos animados
    switch (n) {
      1: "No servir�a de nada.";
      2: "En el �ltimo momento ", (se_) " ech", (as_) " atr�s.";
    }

! Tie:  see JumpOver.
  Touch:
    ! Tocar genera tres mensajes diferentes
    !  1: Si se intenta tocar a un ser animado
    !  2: Tocar un objeto normal del juego
    !  3: Tocarse a si mismo
    switch (n) {
      1: "�Las manos quietas!";
      2: "No not", (as_) " nada extra�o al tacto.";
      3: "Si crees que eso servir� de algo...";
    }
! Turn: see Pull.

  Unlock:
    ! QuitarCerrojo se genera ante ABRE <objeto> CON <objeto2>, o tambi�n
    ! ante QUITA CERROJO A <objeto> (en este segundo caso no se
    ! especifica la "llave" que abre la puerta).
    !
    !  1: Error, el objeto que se intenta abrir, no tiene el atributo
    !    cerrojo.
    !  2: Error, el objeto que se intenta abrir tiene atributo
    !     "cerrojo", pero no tiene atributo "cerrojoechado"
    !  3: Error, el <objeto2> que se intenta usar como llave, no
    !     coincide con la propiedad "con_llave" del <objeto> que
    !     intenta ser abierto.
    !  4: Exito. El <objeto> se abre (con <objeto2> si �ste ha sido
    !     especificado, el cual podemos encontrarlo en la variable "otro"
    switch (n) {
      1: "No parece", (n) x1," tener ning�n tipo de cerrojo.";
      2: print_ret (The) x1, " ya ten�a", (n) x1, " abierto el cerrojo.";
      3: if (second) "No parece", (n) x1, " encajar en la cerradura.";
         "Necesit", (as_) " alg�n tipo de llave.";
      4: if (second) "Quit", (as_) " el cerrojo ", (al) x1, " con ", (the) second, ".";
         "Quit", (as_) " el cerrojo ", (al) x1, ".";
    }

  VagueGo: ! El jugador ha dicho IR sin especificar a d�nde
    "", (_Tienes_) " que especificar en qu� direcci�n ir.";
    
  Verify:
    ! 1: �xito
    ! 2: Error en la verificaci�n
    switch (n) {
      1: "Fichero de juego verificado e intacto.";
      2: "El fichero de juego no parece intacto, puede estar corrompido
         (a menos que est�s jugando con un int�rprete muy primitivo que
         no sea capaz de realizar la comprobaci�n).";
    }
  Wait:      "Pasa el tiempo...";
  Wake:      "La cruda realidad es que esto no es un sue�o.";
  WakeOther: "No parece necesario hacer eso.";
  Wave:
    ! Agitar genera dos mensajes diferentes:
    !  1: Si se intenta agitar un objeto que no est� en el inventario
    !  2: Si se agita un objeto que s� est� en el inventario
    switch (n) {
      1: "No l", (o) x1, " ", (tienes_) ".";
      2: "", (_Te_) " ", (sientes_) " rid�cul", (o) player, " agitando ", (the) x1, ".";
    }
  WaveHands: "", (_Te_) " v", (eo_) " rid�cul", (o) player, " gesticulando as�.";
  ! La acci�n Gesticular sale ante las frases "gesticula", "agita la
  ! mano", "sacude la mano", "saluda con la mano"...
  Wear:
    !  1: Error, el objeto no tiene el atributo "prenda"
    !  2: Error, el jugador no tiene el objeto
    !  3: Error, el objeto ya tiene el atributo "puesto"
    !  4: Exito.
    switch (n) {
      1: "�No ", (puedes_) " poner", (te_) " eso!";
      2: "No ", (lo) x1, " ", (tienes_) ".";
      3: "�Ya ", (lo) x1, " llev", (as_) " puest", (o) x1, "!.";
      4: "", (_Te_) " ", (pones_) " ", (the) x1, ".";
    }
! Yes:  see No.
];
#endif; ! SIN_MENSAJES
!-------------------------------------------------------------------------------
! FIN: Seccion Mensajes
!===============================================================================


! ---------------------------------------------------------------------------
! Otras rutinas de soporte para el AUTOR de Aventuras 

[ TextoLlamativo txt;
    ! Imprime un texto rodeado de asteriscos y en negrita. 
    ! Puedes cambiarlo si quieres que aparezcan en otros formatos.

#ifdef TARGET_ZCODE;
    #IFV5; style bold; #ENDIF;

#ifnot; ! TARGET_GLULX
     glk($0086, 5); ! set alert style
#endif; ! TARGET_
!     print "***", (string) txt, "***"; ! [010508] Corregido bug (string)
     print (string) txt; ! infsp fix, los asteriscos ya se ponian en parserm.h
#ifdef TARGET_ZCODE;
     #IFV5; style roman; #ENDIF;
#ifnot; ! TARGET_GLULX
     glk($0086, 0); ! set normal style
#endif; ! TARGET_
];


! La siguiente funci�n imprime la lista de objetos entre los que el
! parser tiene dudas (porque todos ellos responden al name que ha
! usado el player). Esta rutina es llamada desde los mensajes
! (Miscellany,45) y (Miscellany,46)

! Modificada en [021025] para que pregunte s�lo por aquellos objetos
! empatados a puntuaci�n m�xima por la desambiaguaci�n.

[ ImprimirListaDudosos2 conjuncion
    i k marker punt_max num_objs;
  marker = 0;

  ! obtener puntuacion maxima y cuantos objetos la tienen
  for (i = 0, punt_max = -1: i < number_matched: i++) {
    if (match_scores-->i > punt_max) {
      punt_max = match_scores-->i;
      num_objs = 1;
    }
    else if (match_scores-->i == punt_max)
      num_objs++;
  }

  for (i = 1: i <= number_of_classes: i++)
  {
      while (((match_classes-->marker) ~= i)
             && ((match_classes-->marker) ~= -i)) marker++;
      k=match_list-->marker;

      if (match_scores-->marker == punt_max) {
        if (match_classes-->marker > 0) print (the) k; else print (a) k;

        if (i < num_objs - 1)  print ", "; ! imprimir separadores
        if (i == num_objs - 1) print " ", (string) conjuncion, " ";!imprimir ultimo separador
      }
  }
];

[ ImprimirListaDudosos conjuncion
	i ii k marker punt_max num_objs chk_class;

	marker = 0;

	for (i = 0, punt_max = -1: i < number_matched: i++) {

		if (match_scores-->i > punt_max) {
		  punt_max = match_scores-->i;
		  num_objs = 1;
		}
		else if (match_scores-->i == punt_max)
		  num_objs++;
	}

	chk_class = false;
	if (num_objs >= number_of_classes)
	{
	   num_objs = number_of_classes;
	   chk_class=true;
	}

	for (i = 1, ii = 0 : i <= number_of_classes: i++)
	{

	   while (((match_classes-->marker) ~= i)
		&& ((match_classes-->marker) ~= -i)) marker++;
		k=match_list-->marker;

	    if ((match_scores-->marker == punt_max) || (chk_class == true)) {
		ii++;
		if (match_classes-->marker > 0) print (the) k; else print (a) k;

		if (ii < num_objs - 1) print ", ";
		if (ii == num_objs - 1) print " ", (string) conjuncion, " ";
	    }
	}
]; 
  
! ==============================================================================

Constant LIBRARY_SPANISH;       ! for dependency checking.

! ==============================================================================
