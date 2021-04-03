/*
Mettre ici l'appel de tous les modules
!!!! Doit etre execute meme si la boucle reseau ne fonctionne pas !!!!!!!!

Ameliorer la facon de retourner les info ds la page web si web disponible


 */

char dbgOps = 1; // Normalement 0, mettre a 1 pour la trace des operations
void operations(){
 Serial.println ("\n\n------------ S1m3 mode DEBUG ------------\n\n"); 
 if(dbgOps) {
    delay(5000);
    Serial.println ("\n\n------------ S1m3 mode DEBUG ------------\n\n");
    Serial.print ("\n\n VERSION: ");
    Serial.println (maVersion);
    Serial.println ("\n\n######   Bloc Operations   ######");
  }


  
} // FIN Operations
