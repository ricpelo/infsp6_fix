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
!                Spanish. Grammar Spanish File."
! =====================================================================
!  SpanishG.h -(Gramatica.h) GRAMATICA: Gramática española
!  INFSP  traduccion al ingles de informATE .: by Sarganar :.
!  InformATE es (c) de Zak
!  Re-edicion INFSP by Sarganar
!  caad.es/informate/infsp/
! ----------------------------------------------------------------------------

 Message "Incluyendo SpanishG [INFSP 0.9]";

System_file; ! for supress warnings in I6 environment

#ifdef DEBUG;
  [ ActivarAcentosSub;
    quitacentos = 0;
    "Los acentos no serán eliminados del comando recibido.";
  ];

  [ DesactivarAcentosSub;
    quitacentos = 1;
    "Los acentos serán eliminados del comando recibido, pero sólo en
      las palabras que de otro modo no serían comprendidas.";
  ];
#endif; ! DEBUG

! Elección del dialecto (afecta a la interpretación del
! verbo COGER únicamente)

[ DialectoSub;
  print "Actualmente estás jugando en el dialecto ";
  if (dialecto_sudamericano)
    print "sudamericano";
  else
    print "castellano";
  ". Puedes elegir dialecto usando el comando DIALECTO SUDAMERICANO \
    o bien DIALECTO CASTELLANO.";
];

[ DialectoSudSub;
  dialecto_sudamericano = 2; ! git
  "Elegido dialecto SUDAMERICANO.";
];    

[ DialectoCastSub;
  dialecto_sudamericano = 0;
  "Elegido dialecto CASTELLANO.";
];


[ SubirSub;
  <<Go u_obj>>;
];

[ BajarSub p;         ! 021230
  p=parent(player);
  if ((p~=location) && (p has supporter))
     if (noun~=nothing) <<GetOff noun>>;
     else <<GetOff p>>;
!  print "     Bajar: 2da"; ! infsp debug
  if (noun==nothing)  <<Go d_obj>>;
!  print "     Bajar: 3ra"; ! infsp debug
  if (noun has door) <<Enter noun>>;
!  print "     Bajar: 4ta"; ! infsp debug
  L__M(##Enter,2,noun);
];

[ QuitarSub;
    if (second) <<Remove noun second>>;
    if (noun in player) <<Disrobe noun>>;
    <<Take noun>>;
];

ifdef ADMITIR_COMANDO_SALIDAS;
  [ SalidasSub i flag flag2 j loc;
    print "Salidas visibles:";
    if (location == thedark)
      loc = real_location;
    else
      loc = location;
    j = 0;
    objectloop (i in compass) {
      if (loc provides Salidas)
        switch (loc.Salidas (i)) !se envia el objeto de la brujula como parametro
        {
          false:
            flag  = false;           ! Show la salida si existe la
            flag2 = false;           ! propiedad al_* y no es un string.
          true:
            j++;                     ! La dirección ya ha sido escrita.
            flag2 = true;
          2:
            flag2 = true;            ! No imprimir esta.
          default:
            flag  = true;            ! Imprimirla siempre.
        };  ! de switch

        if (loc provides (i.door_dir) &&
          metaclass (loc.(i.door_dir)) ~= nothing or string ||
          flag == true && flag2 == false)
        {
          if (j == 0)
            print " ";
          else
            print ", ";
          LanguageDirection (i.door_dir);
          j++;
        }  ! de if
    }  ! de objectloop
    if (j == 0)
      " ninguna.";
    ".";        ! ELIUK BLAU: antes hacia dos saltos de linea al final, cuando
                ! en realidad debía hacer solo uno. (el otro salto lo agrega el PROMPT mismo)
  ];
endif;  ! ADMITIR_COMANDO_SALIDAS

! TODO: ¿De verdad son necesarias tantas formas inglesas de los commandos?
! ------------------------------------------------------------------------------
!  The "meta-verbs", commands to the game rather than in the game, come first:
! ------------------------------------------------------------------------------


!#IFNDEF NO_PUNTUACION; !infsp; el parser internamente ya resuelve esta cuestion con NO_SCORE
Verb meta 'score' 'puntos' 'puntuacion'
  *                                   -> Score
  * 'total'/'detallada'/'completa'    -> FullScore;
VerboIrregular "conocer la puntuación" with imperativo 'score' 'puntos' 'puntuacion';

Verb meta 'total' 'pt'
  *                                   -> FullScore;
VerboIrregular "conocer la puntuación detallada" with imperativo 'total' 'pt';
!#ENDIF; ! NO_PUNTUACION

Verb meta 'q//' 'quit' 'terminar' 'fin' 'acabar' 'abandonar'
  *                                   -> Quit;
VerboIrregular "abandonar el juego" with imperativo 'q//' 'quit' 'terminar'
  'fin' 'acabar' 'abandonar';

Verb meta 'recuperar' 'cargar' 'load' 'restaurar' 'restore'
  *                                   -> Restore;
VerboIrregular "cargar un juego previamente guardado en disco" with imperativo
  'cargar' 'recuperar' 'load' 'restaurar' 'restore';

Verb meta 'reiniciar'
  *                                   -> Restart;
VerboIrregular "reiniciar el juego" with imperativo 'reiniciar';

Verb meta 'verificar'
  *                                   -> Verify;
VerboIrregular "verificar la corrección del juego" with imperativo 'verificar';

Verb meta 'save' 'guardar' 'salvar'
  *                                   -> Save;
VerboIrregular "guardar en disco el estado del juego" with imperativo 'save'
  'guardar' 'salvar';

Verb meta 'script' 'transcripcion'
  *                                   -> ScriptOn
  * 'off'/'no'                        -> ScriptOff
  * 'on'/'si'                         -> ScriptOn;
VerboIrregular "activar la transcripción a disco" with imperativo 'script'
  'transcripcion';

Verb meta 'noscript' 'unscript' 'notranscripcion'
  *                                   -> ScriptOff;
VerboIrregular "desactivar la transcripción a disco" with imperativo 'unscript'
  'noscript' 'notranscripcion';

Verb meta 'superbreve' 'corto'
  *                                   -> LMode3;
VerboIrregular "cambiar a modo SUPERBREVE" with imperativo 'superbreve' 'corto';

Verb meta 'verbose' 'largo'
  *                                   -> LMode2;
VerboIrregular "cambiar a modo LARGO" with imperativo 'verbose' 'largo';

Verb meta 'breve' 'normal'
  *                                   -> LMode1;
VerboIrregular "cambiar a modo NORMAL" with imperativo 'breve' 'normal';

Verb meta 'pronombres' 'p//'
  *                                   -> Pronouns;
VerboIrregular "ver el estado de los pronames" with imperativo 'pronombres';

!#IFNDEF NO_PUNTUACION;
Verb meta 'notify' 'notificar' 'notificacion'
  * 'on'/'si'                         -> NotifyOn
  * 'off'/'no'                        -> NotifyOff;
VerboIrregular "cambiar el estado de la NOTIFICACIÓN de PUNTOS" with imperativo
  'notificar' 'notify';
!#ENDIF; ! NO_PUNTUACION

Verb meta 'version'
  *                                   -> Version;
VerboIrregular "conocer el número de VERSIÓN del juego" with imperativo
  'version';

Verb meta 'dialecto'
  *                                   -> Dialecto
  * 'sudamericano'                    -> DialectoSud
  * 'castellano'                      -> DialectoCast;
VerboIrregular "cambiar el dialecto del juego" with imperativo 'dialecto';

#IFNDEF NO_PLACES;
  Verb meta 'places' 'lugares'
    *                                 -> Places;
  VerboIrregular "listar los lugares visitados" with imperativo 'places'
    'lugares';

  Verb meta 'objects' 'objetos'
    *                                 -> Objects;
  VerboIrregular "listar los objetos que se han tenido" with imperativo
 'objects'
    'objetos';
#ENDIF;  ! NO_PLACES

! ----------------------------------------------------------------------------
!  Comandos para depuración
! ----------------------------------------------------------------------------

! TODO: ¿Añadimos formas infinitivas para los commandos de depuración?

#ifdef DEBUG;
  Verb meta 'acentos'
    *                                 -> ActivarAcentos
    * 'on'/'si'                          -> ActivarAcentos
    * 'off'/'no'                      -> DesactivarAcentos;

  Verb meta 'trace' 'traza'
    *                                 -> TraceOn
    * number                          -> Tracelevel
    * 'on'/'si'                       -> TraceOn
    * 'off'/'no'                      -> TraceOff;

  Verb meta 'actions' 'acciones'
    *                                 -> ActionsOn
    * 'on'/'si'                       -> ActionsOn
    * 'off'/'no'                      -> ActionsOff;

  Verb meta 'routines' 'messages' 'rutinas' 'mensajes'
    *                                 -> RoutinesOn
    * 'on'/'si'                       -> RoutinesOn
    * 'off'/'no'                      -> RoutinesOff;

  Verb meta 'recording' 'grabacion'
    *                                 -> CommandsOn
    * 'on'/'si'                       -> CommandsOn
    * 'off'/'no'                      -> CommandsOff;

  Verb meta 'replay' 'reproducir'
    *                                 -> CommandsRead;

  Verb meta 'random' 'aleatorio' 'predecible'
    *                                 -> Predictable;

  Verb meta 'purloin' 'xroba'
    * multi                           -> XPurloin;

  Verb meta 'xmueve' 'abstract'
    * noun 'a//' noun                 -> XAbstract
    * noun 'to' noun                  -> XAbstract;

  Verb meta 'tree' 'arboljuego' 'xarbol'
    *                                 -> XTree
    * noun                            -> XTree;

  Verb meta 'xir' 'irdonde'
    * number                          -> Goto
    * noun                            -> Gonear;

! TODO: ¿Añadimos aquí también 'relojes'?

  Verb meta 'timers' 'daemons' 'temporizadores'
    *                                 -> TimersOn
    * 'on'/'si'                       -> TimersOn
    * 'off'/'no'                      -> TimersOff;

  Verb meta 'changes' 'cambios'
    *                                 -> ChangesOn
    * 'on'/'si'                       -> ChangesOn
    * 'off'/'no'                      -> ChangesOff;

  Verb meta 'gonear'
    * noun                            -> Gonear;
  Verb meta 'goto'
    * number                          -> Goto;

  Verb meta 'scope' 'alcance'
    *                                 -> Scope
    * noun                            -> Scope;

  Verb meta 'xverbo'
    * special                         -> Showverb;

  Verb meta 'xobjeto' 'showobj'
    *                                 -> Showobj
    * number                          -> Showobj ![6/11]
    * multi                           -> Showobj;

  #ifdef TARGET_GLULX;

! TODO: ¿Alguna forma española para este commando? ¿Tal vez 'listaglk'?

    Verb meta 'glklist'
      *                               -> Glklist;
  #endif;  ! TARGET_GLULX
#endif;  ! DEBUG

! ----------------------------------------------------------------------------
!  Los commandos del juego
! ----------------------------------------------------------------------------
Verb 'coge' 'toma' 'recoge'
  * multi                             -> Take
  * 'a//' creature                    -> Take
  * multiinside 'de' noun             -> Remove;

Verb 'saca'
  * multiinside 'de' noun             -> Remove
  * 'a//' creature 'de' noun          -> Remove;

! [001115] El verbo quita genera ahora la acción Quitar, pero
! esta acción es reconvertida por la librería en Disrobe,
! Take o Remove.

Verb 'quita' 'quitale'
  * noun                                                     -> Quitar
  * noun 'de' noun                                           -> Quitar
  * 'a//' creature 'de' noun                                 -> Quitar
  * noun 'a//' noun                                          -> Quitar
  * 'cerrojo'/'pestillo'/'cierre' 'a//' noun                 -> Unlock
  * 'el' 'cerrojo'/'pestillo'/'cierre' 'a//' noun            -> Unlock
  * 'cerrojo'/'pestillo'/'cierre' 'a//' noun 'con' held      -> Unlock
  * 'el' 'cerrojo'/'pestillo'/'cierre' 'a//' noun 'con' held -> Unlock;
VerboIrregular "quitarle" with imperativo 'quitale';

Verb 'sacate' 'quitate' 'sacarse' 'quitarse' 'quitarte' 'sacarte' 'sacarme' 'quitarme' 'quitame' 'sacame'
  * noun                              -> Disrobe;
VerboIrregular "quitar" with imperativo 'sacate' 'quitate' 'sacarse' 'quitarse'
  'quitarte' 'sacarte' 'sacarme' 'quitarme' 'quitame' 'sacame';

Verb 'ponte' 'viste' 'vistete' 'ponerse' 'vestirse' 'ponerte' 'vestirte' 'ponerme' 'vestirme' 'ponme' 'visteme'
  * held                              -> Wear
  * 'con' held                        -> Wear;
VerboIrregular "poner" with imperativo 'pon' 'ponte' 'viste' 'vistete'
  'ponerse' 'vestirse' 'ponerte' 'vestirte' 'ponerme' 'vestirme' 'ponme' 'visteme';

Verb 'pon' 'mete' 'echa' 'inserta' 'coloca'
  * multiexcept 'en' container                              -> Insert
  * multiexcept 'en' noun                                    -> PutOn
  * multiexcept 'dentro' 'de' noun                           -> Insert
  * multiexcept 'sobre' noun                                 -> PutOn
  * noun 'a//' topic                                         -> SetTo
  * 'a//' creature 'en' container                           -> Insert
  * 'a//' creature 'en' noun                                 -> PutOn
  * 'a//' creature 'dentro' 'de' noun                        -> Insert
  * 'a//' creature 'sobre' noun                              -> PutOn
  * multiexcept 'encima' 'de' noun                           -> PutOn
  * 'a//' creature 'encima' 'de' noun                        -> PutOn
  * 'cerrojo'/'pestillo'/'cierre' 'a//' noun                 -> Lock
  * 'el' 'cerrojo'/'pestillo'/'cierre' 'a//' noun            -> Lock
  * 'cerrojo'/'pestillo'/'cierre' 'a//' noun 'con' held      -> Lock
  * 'el' 'cerrojo'/'pestillo'/'cierre' 'a//' noun 'con' held -> Lock;

Verb 'vacia'
    * noun                               -> Empty
    * noun 'dentro' 'de' noun            -> EmptyT
    * noun 'en' noun                     -> EmptyT
    * noun 'sobre' noun                  -> EmptyT
    * noun 'encima' 'de' noun            -> EmptyT;
Verb 'transfiere' 'cambia' 
    * noun 'a//' noun                     -> Transfer;
VerboIrregular "transferir"  with imperativo 'transfiere';
Verb 'deja' 'suelta'
    * multiheld                          -> Drop
    * 'a//' creature            -> Drop
    * multiexcept 'en' container         -> Insert
    * multiexcept 'en' noun             -> PutOn
    * multiexcept 'dentro' 'de' noun     -> Insert
    * multiexcept 'sobre' noun           -> PutOn
    * multiexcept 'encima' 'de' noun     -> PutOn;
VerboIrregular "soltar" with imperativo 'suelta';
Verb 'lanza' 'arroja'
    * held 'a//' noun                      -> ThrowAt
    * held 'por' noun                    -> ThrowAt
    * 'a//' creature 'por'/'contra' noun-> ThrowAt
    * held 'contra' noun                 -> ThrowAt;
Verb 'tira'
    * 'de' noun                          -> Pull
    * multiheld                          -> Drop
    * multiexcept 'en' container         -> Insert
    * multiexcept 'en' noun         -> PutOn
    * multiexcept 'dentro' 'de' noun     -> Insert
    * multiexcept 'sobre' noun           -> PutOn
    * multiexcept 'encima' 'de' noun     -> PutOn
    * held 'a//' noun                      -> ThrowAt
    * held 'por' noun                    -> Insert
    * held 'contra' noun                 -> ThrowAt;
Verb 'da' 'regala' 'dale' 'dase' 'ofrece' 'darse'
    * held 'a//' creature        -> Give
    * 'a//' creature held        -> Give reverse
    * creature held            -> Give reverse 
    * 'una' 'patada' 'a//' noun            -> Attack
    * 'un' 'punetazo' 'a//' noun    -> Attack
    * 'un' 'golpe' 'a//' noun              -> Attack;
VerboIrregular "darle" with imperativo 'dale' 'dase' 'darse';
Verb 'muestra' 'ensena' 
    * creature held                     -> Show reverse
    * 'a//' creature held        -> Show reverse
    * held 'a//' creature        -> Show;
VerboIrregular "mostrar" with imperativo 'muestra';
[ ADirection; if (noun in compass) rtrue; rfalse; ];
Verb 'anda' 'camina' 'corre' 'ir' 've' 'vete' 'vuelve'
    *                                   -> VagueGo
    * noun=ADirection                   -> Go
    * 'a//' noun=ADirection           -> Go
    * 'hacia' noun=ADirection           -> Go
    * noun                              -> Enter
    * 'a//' noun                          -> Enter
    * 'hacia' noun                      -> Enter
    * 'por' noun                        -> Enter;
VerboIrregular "ir" with imperativo 've' 'vete' 'ir';
VerboIrregular "volver" with imperativo 'vuelve';

Verb 'pasa'
    * 'por' noun            -> Enter;
Verb 'inventario' 'inv' 'i//' 
    *                                     -> Inv
    * 'breve'/'estrecho'                   -> InvTall
    * 'largo'/'ancho'                      -> InvWide;
VerboIrregular "mostrar inventario" with imperativo 'i//' 'inv' 'inventario';
Verb 'look' 'mira' 'm//' 'ver' 'l//'
    *                                    -> Look
    * 'a//' creature                    -> Examine
    * noun                               -> Examine
    * 'a//'/'hacia' noun        -> Examine
    * 'en' noun                          -> Search
    * 'dentro' 'de' noun                 -> Search
    * 'sobre' noun                       -> Search
    * 'bajo' noun                        -> LookUnder
    * 'debajo' 'de' noun                 -> LookUnder
    * 'a//' 'traves' 'de' noun        -> Search
    * 'por' noun                         -> Search;
VerboIrregular "mirar"  with imperativo 'l//' 'look' 'm//';
Verb 'consulta'
    * creature 'sobre' topic            -> Ask
    * 'a//' creature 'sobre' topic    -> Ask
    * 'sobre' topic 'a//' creature    -> Ask reverse
    * noun 'sobre' topic                -> Consult
    * noun 'acerca' 'de' topic            -> Consult
    * topic 'en' noun            -> Consult reverse;
Verb 'abre'
    * noun                               -> Open
    * 'a//' creature                    -> Open
    * noun 'con' held                    -> Unlock;
VerboIrregular "abrir" with imperativo 'abre';
Verb 'cierra'
    * noun                               -> Close
    * noun 'con' 'pestillo'        -> Lock
    * noun 'con' held                    -> Lock;
VerboIrregular "cerrar"  with imperativo 'cierra';
Verb 'destapa' 'descubre'
    * noun                -> Open;
VerboIrregular "descubrir"  with imperativo 'descubre';
Verb 'tapa' 'cubre'
    * noun                -> Close;
VerboIrregular "cubrir" with imperativo 'cubre';
Verb 'entra' 'cruza'
    *                                    -> GoIn
    * 'en'/'por' noun                      -> Enter ! [infsp] reacomodamiento
!    * 'en' noun                      -> Enter
!    * 'por' noun                      -> Enter
    * 'a//' noun                           -> Enter
    * noun                               -> Enter;
Verb 'metete' 'meterse' 'meterte' 'meterme' 'meteme'
    * 'en'/'por' noun            -> Enter;
VerboIrregular "meter" with imperativo 'metete' 'meterse' 'meterte' 'meterme' 'meteme';
Verb  'atraviesa'
    * noun                -> Enter;
VerboIrregular "atravesar" with imperativo 'atraviesa';
Verb 'sienta' 'echate' 'sientate' 'echarse' 'sentarse' 'echarte' 'sentarte'
    * 'en' noun                          -> Enter;
VerboIrregular "sentar" with imperativo 'sienta' 'sientate' 'sentarse'
    'echarte' 'sentarte';
VerboIrregular "echar" with imperativo 'echate' 'echarse';
Verb 'sal' 'fuera' 'afuera' 'salte' 'bajate' 'levantate' 'bajarse'
    'levantarse' 'salirse' 'bajarte' 'levantarte' 'salirte'
    *                                    -> Exit
    * 'de' noun                          -> GetOff
!    * 'por' noun                         -> Enter
    * 'fuera'                            -> Exit
    * 'afuera'                           -> Exit;
VerboIrregular "salir" with imperativo 'sal' 'fuera' 'afuera' 'salte'
    'salirse' 'salirte';
VerboIrregular "bajar" with imperativo 'bajate' 'bajarse' 'bajarte';
VerboIrregular "levantar" with imperativo 'levantate' 'levantarse'
    'levantarte';
Verb 'baja'                                          ! 021230
    *                                   -> Bajar
    * noun                              -> Bajar
    * 'de' noun                         -> GetOff
    * 'por' noun                        -> Bajar;
Verb 'examina' 'describe' 'inspecciona' 'observa' 'ex' 'x//'
    * noun                               -> Examine
    * 'a//' creature                    -> Examine;
VerboIrregular "examinar" with imperativo 'x//' 'describe' 'ex';
Verb 'lee'
    * noun                               -> Examine
    * 'sobre' topic 'en' noun        -> Consult
    * topic 'en' noun                -> Consult;
Verb 'si' 'sí'
    *                                    -> Yes;
VerboIrregular "responder ~sí~" with imperativo 'si' 'y//';
Verb 'nx'
    *                                    -> No;
VerboIrregular "responder ~no~" with imperativo 'nx';
Verb 'sorry' 'perdon' 'perdona' 'disculpa'
    *                                   -> Sorry
    * topic                             -> Sorry;
Verb 'siento' 'lamento'
    * topic                             -> Sorry;
Verb 'lo'
    * 'siento'/'lamento'                -> Sorry
    * 'siento'/'lamento' topic          -> Sorry;
!VerboIrregular "disculparte" with imperativo 'sorry' 'perdon';

Verb 'mierda' 'jode' 'joder' 'puta' 'cono' 'cabron' 'puto'
    *                                   -> Strong
    * topic                            -> Strong;
Verb 'aburrido' 'tonto' 'bobo' 'idiota'
    *                                    -> Mild
    * topic                            -> Mild;
Verb 'busca'
    * 'en' noun                          -> Search
    * topic 'en' noun                 -> Consult
    * 'en' noun 'sobre' topic        -> Consult
    * 'en' noun topic            -> Consult
    * 'en' noun 'acerca' 'de' topic    -> Consult;
Verb 'registra' 'rebusca'
    * 'a//' creature                    -> Search
    * noun                -> Search
    * 'en' noun                -> Search;
Verb 'ondea' 'sacude' 'agita'
    * creature                          -> Attack
    * 'a//' creature                    -> Attack
    * 'la' 'mano'                       -> WaveHands
    * 'las' 'manos'                     -> WaveHands
    * noun                               -> Wave;
VerboIrregular "sacudir" with imperativo 'sacude';
Verb 'gesticula'
    *                                   -> WaveHands;
Verb 'saluda'
    * 'con' 'la' 'mano'            -> WaveHands;
Verb 'ajusta' 'fija' 'set'
    * noun                               -> Set
    * noun 'en'/'a//' topic        -> SetTo;
VerboIrregular "ajustar" with imperativo 'set';
Verb 'pulsa'
    * noun                -> Push;
Verb 'empuja' 'mueve' 'desplaza' 'menea'
    * noun                               -> Push
    * multi 'a//' noun                -> Transfer
    * 'a//' creature                    -> Push
    * noun 'hacia' noun                  -> PushDir
    * noun noun                          -> PushDir;
VerboIrregular "mover" with imperativo 'mueve';
Verb 'gira'  'atornilla' 'desatornilla'
    * noun                               -> Turn;
Verb 'conecta'
    * noun                               -> SwitchOn
    * noun 'a//'/'con' noun         -> Tie;
Verb 'enciende' 'prende'
    * switchable            -> SwitchOn
    * noun                -> Burn;
VerboIrregular "encender" with imperativo 'enciende';
Verb 'desconecta' 'apaga'
    * noun                               -> SwitchOff
    * 'a//' creature                    -> SwitchOff;
Verb 'rompe' 'aplasta' 'golpea'
    'destruye' 'patea' 'pisotea'
    * noun                               -> Attack
    * 'a//' creature            -> Attack;
VerboIrregular "destruir" with imperativo 'destruye';
Verb 'ataca' 'mata' 'asesina' 'tortura' 'noquea'
    * 'a//' creature                    -> Attack
    * noun                              -> Attack;
Verb 'lucha'
    * 'con' creature                    -> Attack
    * creature                          -> Attack;
Verb 'espera' 'z//'
    *                                    -> Wait;
VerboIrregular "esperar" with imperativo 'z//';
Verb 'responde' 'di' 'grita' 'dile'
    * 'a//' creature topic              -> Answer reverse ! infsp fix , delete 'reverse'
    * topic 'a//' creature        -> Answer ;
VerboIrregular "decir" with imperativo 'di' 'dile';
Verb 'cuenta' 'narra' 'explica' 'habla'
    * creature 'de' topic        -> Tell
    * creature 'sobre' topic        -> Tell
    * creature topic                       -> Tell
    * 'a//' creature 'de'/'sobre' topic -> Tell
    * 'a//' creature  topic        -> Tell
    * 'con' creature 'sobre' topic    -> Tell
    * 'con' creature 'de' topic        -> Tell
    * 'con' creature 'acerca' 'de' topic -> Tell
    * 'sobre' topic 'con' creature    -> Tell reverse
    * 'acerca' 'de' topic 'con' creature -> Tell reverse
    * 'de' topic 'con'/'a//' creature    -> Tell reverse
    * topic 'a//' creature        -> Tell reverse;
VerboIrregular "contar" with imperativo 'cuenta';
Verb 'pregunta' 'interroga'
    * creature 'sobre'/'por' topic      -> Ask
    * 'a//' creature 'sobre'/'por' topic -> Ask
    * 'sobre'/'por' topic 'a//' creature -> Ask reverse
    * topic 'a//' creature        -> Ask reverse
    * 'a//' creature 'acerca' 'de' topic -> Ask;
Verb 'pide' 'pidele'
    * 'a//' creature noun        -> AskFor
    * noun 'a//' creature        -> AskFor reverse;
VerboIrregular "pedir" with imperativo 'pide' 'pidele';
Verb 'come' 'comete' 'traga' 'ingiere' 'mastica' 'comerse' 'comerte'
    * held                -> Eat;
VerboIrregular "comer" with imperativo 'comete' 'comerse' 'comerte';
VerboIrregular "ingerir" with imperativo 'ingiere';

Verb 'duerme' 'ronca' 'descansa'
    *                    -> Sleep;
VerboIrregular "dormir" with imperativo 'duerme';
Verb 'canta'
    *                    -> Sing;
Verb 'escala' 'trepa' 
    * 'a//' noun            -> Climb
    * noun                                -> Climb
    * 'por' noun                          -> Climb;
Verb 'subete' 'subirse' 'subirte'
    * 'a//'/'en' noun            -> Enter;
Verb 'sube'
    *                                   -> Subir
    * noun                              -> Enter   ! 021230
    * 'a//' / 'en' / 'por'  noun        -> Enter;
VerboIrregular "subir" with imperativo 'sube' 'subete' 'subirse' 'subirte';
Verb 'compra' 'adquiere'
    * noun                               -> Buy;
VerboIrregular "adquirir" with imperativo 'adquiere';
Verb 'retuerce' 'aprieta' 'estruja' 'tuerce'
    * noun                               -> Squeeze
    * 'a//' creature                    -> Squeeze;
VerboIrregular "apretar" with imperativo 'aprieta';
VerboIrregular "retorcer" with imperativo 'retuerce';
VerboIrregular "torcer" with imperativo 'tuerce';

Verb 'nada'
    *                                    -> Swim;
Verb 'balanceate' 'columpiate' 'meneate' 'balancearse' 'columpiarse'
    'menearse' 'balancearte' 'columpiarte' 'menearte'
    * 'en' noun                          -> Swing;
VerboIrregular "balancear" with imperativo 'balanceate' 'meneate'
    'balancearse' 'menearse' 'balancearse' 'menearse';
VerboIrregular "columpiar" with imperativo 'columpiate' 'columpiarse'
 'columpiarse';
Verb 'sopla'
    * noun                               -> Blow;
Verb 'reza' 'ora'
    *                                    -> Pray;
Verb 'despierta' 'espabila' 
    *                                    -> Wake
    * creature                           -> WakeOther
    * 'a//' creature                    -> WakeOther;
Verb 'espabilate' 'espabilarse' 'espabilarte'
    *                    -> Wake;
VerboIrregular "despertar" with imperativo 'despierta' 'espabilate'
    'espabilarse' 'espabilarte';
Verb 'besa' 'abraza'
    * creature                           -> Kiss
    * 'a//' creature                    -> Kiss;
Verb 'piensa'
    *                                    -> Think;
VerboIrregular "pensar" with imperativo 'piensa';
Verb 'huele' 'olfatea'
    *                                    -> Smell
    * 'a//' noun            -> Smell
    * noun                               -> Smell;
VerboIrregular "oler" with imperativo 'huele';
Verb 'escucha' 'oye'
    *                                    -> Listen
    * 'a//' noun            -> Listen
    * noun                               -> Listen;
VerboIrregular "oir" with imperativo 'oye';
Verb 'saborea' 'paladea' 'prueba' 'lame'
    * noun                               -> Taste
    * 'a//' noun            -> Taste;
VerboIrregular "probar" with imperativo 'prueba';
Verb 'toca' 'palpa'
    * noun                               -> Touch
    * 'a//' creature                    -> Touch;
Verb 'lava' 'limpia' 'pule' 'abrillanta' 'friega' 'frota'
    * noun                               -> Rub
    * 'a//' creature                    -> Rub;
VerboIrregular "pulir" with imperativo 'pule';
VerboIrregular "fregar" with imperativo 'friega';
Verb 'ata' 'enlaza' 'enchufa' 'une'
    * noun                               -> Tie
    * 'a//' creature                    -> Tie
    * 'a//' creature 'a//' noun         -> Tie
    * noun 'a//' noun                      -> Tie;
VerboIrregular "unir" with imperativo 'une';
Verb 'quema'
    * noun                               -> Burn
    * 'a//' creature                    -> Burn
    * 'a//' creature 'con' held         -> Burn
    * noun 'con' held                    -> Burn;
Verb 'bebe'
    * noun                               -> Drink;
Verb 'llena' 'rellena'
    * noun                           -> Fill;
Verb 'corta' 'rasga' 
    * noun                               -> Cut
    * noun 'con' held                    -> Cut;
Verb 'salta' 'brinca'
    *                                    -> Jump
    * noun                               -> JumpOver
    * 'a//' noun                          -> Enter
    * 'sobre' noun                       -> JumpOver
    * 'por' 'encima' 'de' noun        -> JumpOver;
Verb 'cava' 'excava'
    * 'en' noun                -> Dig
    * noun                              -> Dig
    * noun 'con' held                   -> Dig
    * 'en' noun 'con' held              -> Dig;

ifdef ADMITIR_COMANDO_SALIDAS;
! A partir de la revisión [021025] el verbo Salidas ya no es metaverbo.

  Verb 'salidas' 'exits'
    *                                   -> Salidas;
  Extend only 'x//' first
    *                                   -> Salidas;
VerboIrregular "mostrar las salidas" with imperativo 'salidas'; ![infsp] Urba touch

endif;  ! ADMITIR_COMANDO_SALIDAS


! Nuevo en la revisión [001101]. Xir es análogo a Xarbol pero no incluye
! en la lista los objetos de librería ni los verbos irregulares
#ifdef DEBUG;
[ XlistaSub i;
    objectloop(i) 
          if (i ofclass Object && parent(i)==0){
         if (i ofclass VerboIrregular) continue;
        if (i==compass or InformParser
        or InformLibrary or LibraryMessages) continue;
        Xobj(i);
    }
];
Verb meta 'xlista'
    *                    -> XLista;
#endif;

! ----------------------------------------------------------------------------
!  Final task: provide trivial routines if the user hasn't already:
! ----------------------------------------------------------------------------

#Stub TimePasses    0;
#Stub Amusing    0;
#Stub DeathMessage   0;
#Stub DarkToDark 0;
#Stub NewRoom      0;
#Stub LookRoutine     0;
#Stub AfterLife         0;
#Stub GamePreRoutine  0;
#Stub GamePostRoutine 0;
#Stub AfterPrompt    0;
#Stub BeforeParsing   0;
#Stub PrintTaskName  1;
#Stub InScope       1;
#Stub UnknownVerb 1;
#Stub PrintVerb    1;
#Stub ParserError     1;
#Stub ParseNumber 2;
#Stub ChooseObjects   2;
#Stub PreguntarPreposicion 0;
#Stub MirarHaciaSub   1; ! [INFSP TODO] esto va?

#Ifndef PrintRank;
! Constant Make__PR;
! #Endif;
! #Ifdef Make__PR;
[ PrintRank; "."; ];
#Endif;

#Ifndef ParseNoun; ! Si no se incluyo una ParseNoun propia antes en el codigo....
! Constant Make__PN;
! #Endif;
! #Ifdef Make__PN;

! La rutina ParseNoun se ocupa de ignorar "de" cuando aparece entre
! dos palabras reconocidas como names de un mismo objeto. Así, si
! un objeto tiene en su campo "name" los valores "caja" "madera" "caoba"
! ParseNoun aceptará "caja de madera" "caja de caoba" e incluso
! "caja de madera de caoba". En cambio, ante la cadena "caja de bolsa", sólo
! aceptará la primera palabra (caja) dejando sin procesar "de bolsa", lo
! que permite la posterior interpretacion de frases como "saca caja de bolsa"
! E incluso "saca caja de madera de caoba de bolsa"
! De paso también ignora artículos con lo que también se admite:
! "saca la caja de madera de caoba de la bolsa"
!
! ParseNoun debe retornar un número que indica cuántas palabras admite
! como pertenecientes al objeto. En nuestro ejemplo, ante "caja de madera"
! debe retornar 3, pero ante "caja de bolsa" debe retornar 1.
!
[ ParseNoun obj n dudas seguir gen p aux;

    n=0;       ! numero de palabras reconocidas de momento
    seguir=1;  ! =1 seguir mirando palabras para ver si las siguientes
               ! se refieren a este mismo objeto

    aux=aux;
   #ifdef DEBUG;
    aux=debug_flag;
    debug_flag=0; ! Desactivar trazado de give (para no cansar con tanto 'nombreusado' cambiando continuamente)
   #endif;
    give obj ~nombreusado; ! resetear nombreusado (quitar nombreusado)
    gen=5;    ! 1=masculino,        2=femenino,
              ! 3=masculino plural, 4=femenino plural
              ! 5=sin determinar

    while(seguir)
    {
      p=NextWord();
      if ((p=='de')&&(n==0)) return 0;
    
!     if (WordInProperty(p, obj, name))
!         ! Una palabra que se refiere al objeto
!     {
!         n++;         ! la contamos
!         n=n+dudas;   ! añadimos todos los "de" "la"...
!                      ! que estaban sin contar
!         dudas=0;     ! y resetamos el contador de "de"s
!         continue;    ! volvemos al while
!     }
      if (WordInProperty(p, obj, name_f))
          ! Una palabra que se refiere al objeto
      {
        if (p) dict_flags_of_noun = (p->#dict_par1) & $$01110100;
        if (gen==5) gen=G_FEMENINO;
        n++;            ! la contamos
        n=n+dudas;      ! añadimos todos los "de" "la"...
                        ! que estaban sin contar
        dudas=0;        ! y resetamos el contador de "de"s
!        print " --> es ahora femenino...." ; !infsp debug
        continue;       ! volvemos al while
      }
      if (WordInProperty(p, obj, name_mp))
          ! Una palabra que se refiere al objeto
      {
        if (p) dict_flags_of_noun = (p->#dict_par1) & $$01110100;
        if (gen==5) gen=G_MASCULINO+G_PLURAL;
        n++;            ! la contamos
        n=n+dudas;      ! añadimos todos los "de" "la"...
                        ! que estaban sin contar
        dudas=0;        ! y resetamos el contador de "de"s
        continue;       ! volvemos al while
      }
      if (WordInProperty(p, obj, name_fp))
      {
        if (p) dict_flags_of_noun = (p->#dict_par1) & $$01110100;
        if (gen==5) gen=G_FEMENINO+G_PLURAL;
        n++;            ! la contamos
        n=n+dudas;      ! añadimos todos los "de" "la"...
                        ! que estaban sin contar
        dudas=0;        ! y resetamos el contador de "de"s
        continue;       ! volvemos al while
      }
    ! [I7] se cambio de lugar este if para buscar el name_m (o name)
    ! en ultimo lugar, debido a una interferencia q provoca la asignacion 'automatica' de 
    ! plural en los objetos I7
      if (WordInProperty(p, obj, name_m))
            ! Una palabra que se refiere al objeto
      {
        if (p) dict_flags_of_noun = (p->#dict_par1) & $$01110100;
        if (gen==5) gen=G_MASCULINO;
        n++;            ! la contamos
        n=n+dudas;      ! añadimos todos los "de" "la"...
                        ! que estaban sin contar
        dudas=0;        ! y resetamos el contador de "de"s
!        print " --> es ahora masculino...." ; !infsp debug
        continue;       ! volvemos al while
      }
      if (WordInProperty(p, obj, adjectives))
      {
        if (p) dict_flags_of_noun = (p->#dict_par1) & $$01110100;
        n++;            ! la contamos
        n=n+dudas;      ! añadimos todos los "de" "la"...
                        ! que estaban sin contar
        dudas=0;        ! y resetamos el contador de "de"s
!        print (the) obj," --> adjetivo detectado....^" ; !infsp debug
        continue;       ! volvemos al while
      }
        
    ! Si la palabra no fue reconocida, miraremos si se trata
    ! de 'de' o un artículo. En este caso mantenemos la duda y
    ! seguimos mirando palabras, hasta que una de ellas encaje
    ! en este objeto$ en cuyo caso se retornan todas las que se
    ! han leido, o bien se encuentre una que no encaja en cuyo
    ! caso se retorna las que se habían encontrado antes del "de"

      else if (p== 'el' or 'la' or 'los' or 'las' or
         'de')
      {
        dudas++; continue;
      }        
      else seguir=0;   ! Si no fue reconocida, ni un articulo, ni "de"
                       ! nos damos por vencidos
   }! while block
    
    if (obj provides gender)
        switch(gen)
        { ! Los casos del switch estaban mal.
          ! Bug corregido en 001030
          1: give obj ~female ~pluralname; !infsp fix for I7 compatibility (en I7 no se puede asignar el 0 a gender, de 1 para arriba, sí)
          2: give obj female ~pluralname;  !print " FEMALE GIVEN "; !infsp debug
          3: give obj ~female pluralname;
          4: give obj female pluralname;
        }

    if (gen<5) give obj nombreusado; !si el matcheo fue por adjetives, entonces gen=5
                                     !   y nombreusado no se pone.                            
!!    if (obj provides gender) obj.gender=gen; ! esta linea siempre esta comentariada
    
#ifdef DEBUG;
    debug_flag=aux;
#endif;
!    if (parser_trace >= 7) ! imprimir el buffer recibido
!    {
!          print "^[ ParseNoun:  ";
!          print "^      consulta por ", (the) obj, " --> palabras reconocidas ",n ;
!    } 
    return n; !retornar numero de palabras reconocidas
    !return -1;
];    

#Endif; ! def ParseNoun

#Default Story 0;
#Default Headline 0;

#ifdef TARGET_GLULX;
#Stub IdentifyGlkObject 4;
#Stub HandleGlkEvent  2;
#Stub InitGlkWindow   1;
#endif; ! TARGET_GLULX

! [010515] Cambiado de sitio el punto de inclusión de Mensajes.h
! para dar la oportunidad al usuario de incluir su proper Mensajes
! si lo desea (antes del include Gramatica)
#ifndef LanguageLM;
!include "Mensajes"; ! infsp withdraw
#endif;

Include "INFSPR"; ! [INFSP] Inform Parser Replacement/Hack File


#Ifdef INFIX;
 #Include "infix";
#Endif;

! ==============================================================================

Constant LIBRARY_GRAMMAR;       ! for dependency checking

! ==============================================================================
