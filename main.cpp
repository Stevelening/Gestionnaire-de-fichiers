#include <QApplication>
#include "MaFenetre.h"
#include "TableView.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MaFenetre fenetre;    //on cree un objet de type Mafenetre
  fenetre.show();       // on affiche la fenetre

  return app.exec();    // cette ligne permet au programme d'etre compile
}

