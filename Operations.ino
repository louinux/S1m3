/*
  Mettre ici l'appel de tous les modules
  !!!! Doit etre execute meme si la boucle reseau ne fonctionne pas !!!!!!!!

  Ameliorer la facon de retourner les info ds la page web si web disponible


*/

char dbgOps = 1; // Normalement 0, mettre a 1 pour la trace des operations
void operations() {
  if (dbg || dbgOps) {
    Serial.println ("Boucle Operations");
    delay(ddbg);
  }
  sondeTH();

  if (dbg || dbgOps) {
    Serial.println ("FIN Boucle Operations");
    delay(ddbg);
  }
} // FIN Operations
