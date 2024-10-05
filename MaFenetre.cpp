#include"MaFenetre.h"

QList<MaFenetre*> MaFenetre::tableau;                      // comme ces attributs sont static alors on doit les redefinir avant l'implementations des methodes
QString MaFenetre::fileName;
QString MaFenetre::filePath;
QModelIndex MaFenetre::ind1;
QModelIndex MaFenetre::ind2;

MaFenetre::MaFenetre(QWidget *parent) : QMainWindow(parent)                                          // implementation du constructeur de ma fenetre
{
    QWidget *zoneCentrale = new QWidget;                      // creation d'un objet de type QWidget qui en fait la zone centrale de ma fenetre
    setWindowTitle(tr("FileManager"));             //titre de la fenetre
    setWindowIcon(QIcon(":/images/fileManager.png"));            // on definit l'icone de lápplication
    statusBar()->showMessage("Bienvenue");                        //message qui s'affiche dans la barre d'etat au lancement du programme
    this->resize(800,600);                                      //on impose que la fenetre s'affiche avec ces dimensions au depart mais on peut ensuite la redimensionner a sa guise
    MaFenetre::tableau.append(this);                              // a chaque fois qu'une instance de la classe MaFenetre est cree ,son pointeur est durectement ajoute au tableau dynamyque

    qDebug() << tableau.count() ;                                 // pour tester si les elements sont bien ajoutes dans 'tableau'


    m_bouton1 = new QPushButton("Prec.");                       // implementation des boutons
    m_bouton2 = new QPushButton("Suiv.");
    m_bouton3 = new QPushButton("Chercher");
    m_bouton4 = new QPushButton("Parent");
    m_bouton5 = new QPushButton("Ouvrir l'emplacement");
    m_lineEdit1 = new QLineEdit("");
    m_lineEdit2 = new QLineEdit("");
    label = new QLabel("                      Aller à :       ");
    m_lineEdit2->setPlaceholderText("Saisir ici pour rechercher");     //texte de fond
    m_bouton1->setToolTip("Precedent");
    m_bouton2->setToolTip("Suivant");
    m_bouton3->setIcon(QIcon(":/images/search.png"));
    m_bouton3->setToolTip("Cliquer ici pour lancer la recherche");
    m_bouton4->setToolTip("Revenir au dossier parent");                                     // l'info bulll
    m_bouton5->setToolTip("Ouvrir l'emplacement du fichier");
    QObject::connect(m_bouton4,SIGNAL(clicked()),this,SLOT(on_m_bouton4_clicked()));       // on cree des connections entres les boutons et les slots
    QObject::connect(m_bouton1,SIGNAL(clicked()),this,SLOT(on_m_bouton1_clicked()));
    QObject::connect(m_bouton2,SIGNAL(clicked()),this,SLOT(on_m_bouton2_clicked()));
    QObject::connect(m_bouton3,SIGNAL(clicked()),this,SLOT(on_m_bouton3_clicked()));
    QObject::connect(m_bouton5,SIGNAL(clicked()),this,SLOT(on_m_bouton5_clicked()));


    m_treeView = new QTreeView();
    m_tableView = new TableView(this);                           // implementations des vues
    m_tableView->setShowGrid(false);

    m_tableView1 = new TableView(this);
    m_tableView1->setEditTriggers(QAbstractItemView::NoEditTriggers);     // empecher la modiffication des cellules du tableau

    split1 = new QSplitter;
    split2 = new QSplitter;
    m_vlayout = new QVBoxLayout;                            // implementatios des conteneurs
    m_hlayout1 = new QHBoxLayout;
    m_hlayout2 = new QHBoxLayout;
    m_hlayout3 = new QHBoxLayout;
    m_hlayout1->addWidget(m_bouton1);                        // on ajoute les boutons dans les conteneurs
    m_hlayout1->addWidget(m_bouton2);
    m_hlayout1->addWidget(m_bouton4);

    split2->addWidget(m_lineEdit1);
    split2->addWidget(m_lineEdit2);
    m_hlayout3->addWidget(split2);
    m_hlayout1->addLayout(m_hlayout3);

    m_hlayout1->addWidget(m_bouton3);

    QString sPath = "C:/";
    modele = new QFileSystemModel(this);                     // on implemente le modele
    modele->setRootPath(sPath);                               // on affiche le parent du disque C dans le modele
    m_treeView->setModel(modele);                            // affiche le modele suivant la vue en abre

    filemodele = new QFileSystemModel(this);
    filemodele->setRootPath(sPath);
    m_tableView->setModel(filemodele);                       // affiche le modele suivant la vue en table

    split1->addWidget(m_treeView);
    split1->addWidget(m_tableView);
    m_hlayout2->addWidget(split1);

    m_vlayout->addLayout(m_hlayout1);
    m_vlayout->addLayout(m_hlayout2);

    zoneCentrale->setLayout(m_vlayout);                      // on met le layout vertical dans la zone centrale de la fenetre

    setCentralWidget(zoneCentrale);

    createActions();
    createMenus();

    QObject::connect(m_treeView,SIGNAL(activated(const QModelIndex &)),this,SLOT(on_m_treeView_activated(const QModelIndex &)));
    QObject::connect(m_tableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(on_m_tableView_clicked(const QModelIndex &)));
    QObject::connect(m_treeView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(on_m_treeView_clicked(const QModelIndex &)));
    QObject::connect(m_tableView,SIGNAL(activated(const QModelIndex &)),this,SLOT(on_m_tableView_activated(const QModelIndex &)));
    QObject::connect(m_tableView,SIGNAL(clickDroit(QContextMenuEvent *)),this,SLOT(on_clickDroit_activated(QContextMenuEvent *)));
    QObject::connect(m_tableView1,SIGNAL(activated(const QModelIndex &)),this,SLOT(on_m_tableView1_activated(const QModelIndex &)));


    actionSupprimer->setEnabled(false);    // on veut que au lancement du programme ces actions soit grisees c'est a dire desactive pour un debut
    actionColler->setEnabled(false);
    actionRenommer->setEnabled(false);
    actionCopier->setEnabled(false);
    actionCouper->setEnabled(false);

}


void MaFenetre::on_actionQuitter_triggered()            // slots qui permet de fermer le programme
{
    qApp->quit();
}

void MaFenetre::on_m_treeView_activated(const QModelIndex &index)     // slots lorsqu'on double-clic dans la treeView
{
  QString sPath = modele->fileInfo(index).absoluteFilePath();         //on recupere le chemin absolu de l'element sur lequel on a double-clique
  m_tableView->setRootIndex(filemodele->setRootPath(sPath));
  if(pos<(liste.count()-1))
  {
      for(i=pos+1;i<= (liste.count()-1);i++)
      {
          liste.removeAt(i);
      }
  }
  liste.append(sPath);      // on ajoute ce chemin dans le tableau liste
  pos=pos+1;                   // pos s'incremente

}


void MaFenetre::on_m_tableView_clicked(const QModelIndex &index)
{
 QString sPath = modele->fileInfo(index).absoluteFilePath();
 m_lineEdit1->setText(sPath);                                               // on affiche ce chemin dans la zone de texte
 actionSupprimer->setEnabled(true);                                        // lorsqu'un element est selectionne , on active les actions jadis desactivees
 actionRenommer->setEnabled(true);
 actionCopier->setEnabled(true);
 actionCouper->setEnabled(true);
 table = m_tableView->SelectedIndexes();

 ind1 = index;                     // on garde l'index de l'element selectionne

}

void MaFenetre::on_m_bouton5_clicked()
{
  table = m_tableView1->SelectedIndexes();
  int a = table[0].row();
  qDebug() << a ;



  QString sPath = model->data(model->index(a,2)).toString();
  qDebug() << sPath;


  m_lineEdit1->setText(sPath);                                               // on affiche ce chemin dans la zone de texte

  QFileInfo fileInfo(sPath);

  QDir parent = fileInfo.dir();

  sPath = parent.absolutePath();

  qDebug() << sPath;

  m_tableView->setRootIndex(filemodele->setRootPath(sPath));



  if(pos<(liste.count()-1))
  {
      for(i=pos+1;i<= (liste.count()-1);i++)
      {
          liste.removeAt(i);
      }
  }
  liste.append(sPath);      // on ajoute ce chemin dans le tableau liste
  pos=pos+1;                   // pos s'incremente
  m_lineEdit1->setText(sPath);
  table.clear();

  fenetreRecherche->close();

}


void MaFenetre::on_m_treeView_clicked(const QModelIndex &index)
{

    QString sPath = modele->fileInfo(index).absoluteFilePath();
    m_lineEdit1->setText(sPath);

    actionSupprimer->setEnabled(true);                      // lorsqu'un element est selectionne , on active les actions jadis desactivees
    actionRenommer->setEnabled(true);
    actionCopier->setEnabled(true);
    actionCouper->setEnabled(true);
}


void MaFenetre::ouvrir_fichier(const char* FilePath)
{
   ShellExecuteA(GetDesktopWindow(),"open",FilePath,"","",SW_SHOW);          // on fait un appel systeme pour ouvrir un fichier
}


void MaFenetre::on_m_tableView_activated(const QModelIndex &index)
{

   QString sPath = modele->fileInfo(index).absoluteFilePath();
   if(modele->fileInfo(index).isFile())                                       // si l'element selectionne est un fichier on l'ouvre avec ShellExecuteA
   {
       const char* spath = sPath.toStdString().c_str();                       // comme la methode SheelExecuteA attend un char*, alorsb on convertit le sPath en un char*
       ouvrir_fichier(spath);
   }
   else                                                                         // si l'element selectionne est un dossier on l'ouvre simplement
   {
       m_tableView->setRootIndex(filemodele->setRootPath(sPath));
       if(pos<(liste.count()-1))
       {
           for(i=pos+1;i<= (liste.count()-1);i++)
           {
               liste.removeAt(i);
           }
       }

       liste.append(sPath);
       pos=pos+1;
   }

}

void MaFenetre::on_m_tableView1_activated(const QModelIndex &index)
{
    int a = index.row();
    QString sPath = model->data(model->index(a,2)).toString();
    qDebug() << sPath;

    const char* spath = sPath.toStdString().c_str();                       // comme la methode SheelExecuteA attend un char*, alorsb on convertit le sPath en un char*    ouvrir_fichier(spath);
    ouvrir_fichier(spath);

}


void MaFenetre::on_actionOuvrir_triggered()
{
   QString fichier = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"), "/home", "");
}


void MaFenetre::on_actionEnregistrer_triggered()
{
   QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "");
}

void MaFenetre::recherche(QFileInfo fileInfo, QStandardItemModel *model, QStringList filtres)
{

    QDir parent(fileInfo.absoluteFilePath());
    QDir parent1(fileInfo.absoluteFilePath());

    parent.setNameFilters(filtres);
    parent1.setFilter(QDir::Dirs);
    parent.setSorting(QDir::Size | QDir::Reversed);
    parent1.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList tab = parent.entryInfoList();
    QFileInfoList tab1 = parent1.entryInfoList();

    qDebug() << tab.size();
    qDebug() << tab1.size();

    for(int t=0;t<tab.size();t++)
    {
        QFileInfo fileInfo = tab.at(t);
        if(fileInfo.isFile())
        {
            model->setItem(p, 0, new QStandardItem(fileInfo.fileName()));
            int tailleF = fileInfo.size();
            QString taille;
            taille.setNum(tailleF);
            model->setItem(p, 1, new QStandardItem(taille+" octets"));
            model->setItem(p, 2, new QStandardItem(fileInfo.absoluteFilePath()));
            p=p+1;
        }
        else if(fileInfo.isDir())
        {
            QDir parent(fileInfo.absoluteFilePath());
            QFileInfoList tab = parent.entryInfoList();
            int size = tab.size();
            recherche(fileInfo,model,filtres);
            p=p+size+1;
        }


    }

 /*   for(int k=0;k<tab1.size();k++)
    {
        QFileInfo fileInfo1 = tab1.at(k);
        recherche(fileInfo1,model,filtres);

    }     */

}

void MaFenetre::on_m_bouton3_clicked()
{
    if(m_lineEdit2->text()=="")
    {
        return ;
    }
    model = new QStandardItemModel();

    fenetreRecherche = new QDialog(this);                            // on cree une QDialog qui represente la fenetre secondaire
    fenetreRecherche->resize(800,600);                                 // on fixe les dmensions de depart mais elle peuvent etres modifiees par la suite
    m_vlayout1 = new QVBoxLayout;
    m_hlayout11 = new QHBoxLayout;
    m_tableView1 = new TableView(this);
    m_vlayout1->addLayout(m_hlayout11);
    m_vlayout1->addWidget(m_tableView1);

    label1 = new QLabel;
    QStringList filtres;

    m_hlayout11->addWidget(label1);
    m_hlayout11->addWidget(m_bouton5);
    QFileInfo fileInfo(liste[pos]);
    QString s = m_lineEdit2->text();
    filtres<<"*"+s+"*";

    recherche(fileInfo,model,filtres);


    QString taille;
    taille.setNum(p);
    label1->setText("Nombre d'éléments trouvés : "+taille);
    m_tableView1->setModel(model);
    p=0;

    fenetreRecherche->setLayout(m_vlayout1);
    fenetreRecherche->exec();
            // on doit  saisir un element dans la zone de texte 2 et pouvoir faire la recherche de cet element

}

void MaFenetre::on_clickDroit_activated(QContextMenuEvent *event)
{
    QMenu menu(m_tableView);                          // on cree un menu contextuel dont le parent est la tableView
    menu.addAction(actionOuvrirFichier);              //on y ajout plusiers actions
    menu.addAction(actionNouveau);
    menu.addSeparator();                              // on ajoute un separateur
    menu.addAction(actionCouper);
    menu.addAction(actionCopier);
    menu.addAction(actionColler);
    menu.addSeparator();
    menu.addAction(actionRenommer);
    menu.addAction(actionSupprimer);
    menu.addAction(actionProprietes);

    menu.exec(event->globalPos());                       // le menu s'affiche exactement au mme endroit que le clic droit
}

void MaFenetre::on_actionApropos_triggered()
{
    QMessageBox::about(this, tr("A propos de cette Application"),
             tr("Cette <b>Application</b> est un gestionnaire de fichier concue et mis sur pied "
                "par <b>LENING TALAUPA ZIDANE STEVE</b> eleve ingenieur en troisieme annee GI "
                "a l'ENSPY"));
}

void MaFenetre::createActions()
{

    actionNouveau = new QAction("&Nouveau",this);
    actionNouveau->setToolTip("Creer un nouveau dossier");
    actionNouveau->setStatusTip("Creer un nouveau dossier");
    actionNouveau->setShortcut(QKeySequence("Ctrl+N"));
    actionNouveau->setIcon(QIcon(":/images/newDir.png"));
    QObject::connect(actionNouveau,SIGNAL(triggered()),this,SLOT(on_actionNouveau_triggered()));

    actionNouvelleFenetre = new QAction("&Nouvelle Fenetre",this);
    actionNouvelleFenetre->setToolTip("Creer une nouvelle fenetre");
    actionNouvelleFenetre->setStatusTip("Creer une nouvelle fenetre");
    actionNouvelleFenetre->setShortcut(QKeySequence("Ctrl+F"));
    actionNouvelleFenetre->setIcon(QIcon(":/images/new.png"));
    QObject::connect(actionNouvelleFenetre,SIGNAL(triggered()),this,SLOT(on_actionNouvelleFenetre_triggered()));


    actionOuvrir = new QAction("&Ouvrir",this);
    actionOuvrir->setToolTip("Ouvrir un dossier ou un fichier Existant");
    actionOuvrir->setStatusTip("Ouvrir un dossier ou un fichier Existant");
    actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
    actionOuvrir->setIcon(QIcon(":/images/open.png"));
    QObject::connect(actionOuvrir,SIGNAL(triggered()),this,SLOT(on_actionOuvrir_triggered()));


    actionOuvrirFichier = new QAction("&Ouvrir",this);
    actionOuvrirFichier->setToolTip("Ouvrir l'element selectionne");
    actionOuvrirFichier->setStatusTip("Ouvrir l'element selectionne");
    QObject::connect(actionOuvrirFichier,SIGNAL(triggered()),this,SLOT(on_actionOuvrirFichier_triggered()));


    actionEnregistrer = new QAction("&Enregistrer",this);
    actionEnregistrer->setToolTip("Enregistrer les modifications");
    actionEnregistrer->setStatusTip("Enregistrer les modifications");
    actionEnregistrer->setShortcut(QKeySequence("Ctrl+S"));
    actionEnregistrer->setIcon(QIcon(":/images/save.png"));
    QObject::connect(actionEnregistrer,SIGNAL(triggered()),this,SLOT(on_actionEnregistrer_triggered()));



    actionEnregistrerSous = new QAction("&Enregistrer sous...",this);
    actionEnregistrerSous->setToolTip("Enregistrer dans un endroit precis");
    actionEnregistrerSous->setStatusTip("Enregistrer dans un endroit precis");


    actionQuitter = new QAction("&Quitter",this);
    actionQuitter->setToolTip("Quitter le programme");
    actionQuitter->setStatusTip(tr("Quitter le programme"));
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    actionQuitter->setIcon(QIcon(":/images/exit.png"));
    QObject::connect(actionQuitter,SIGNAL(triggered()),this,SLOT(on_actionQuitter_triggered()));


    actionCouper = new QAction(tr("&Couper"),this);
    actionCouper->setToolTip("Couper les elements selectionnes");
    actionCouper->setStatusTip("Couper les elements selectionnes");
    actionCouper->setShortcut(QKeySequence("Ctrl+X"));
    actionCouper->setIcon(QIcon(":/images/cut.png"));
    QObject::connect(actionCouper,SIGNAL(triggered()),this,SLOT(on_actionCouper_triggered()));


    actionCopier = new QAction("&Copier",this);
    actionCopier->setToolTip("Copier les elements selectionnes");
    actionCopier->setStatusTip("Copier les elements selectionnes");
    actionCopier->setShortcut(QKeySequence("Ctrl+C"));
    actionCopier->setIcon(QIcon(":/images/copy1.png"));
    QObject::connect(actionCopier,SIGNAL(triggered()),this,SLOT(on_actionCopier_triggered()));


    actionColler = new QAction("&Coller",this);
    actionColler->setToolTip("Coller les elements copies");
    actionColler->setStatusTip("Coller les elements copies");
    actionColler->setShortcut(QKeySequence("Ctrl+V"));
    actionColler->setIcon(QIcon(":/images/paste1.png"));
    QObject::connect(actionColler,SIGNAL(triggered()),this,SLOT(on_actionColler_triggered()));

    actionSupprimer = new QAction("&Supprimer",this);
    actionSupprimer->setToolTip("Supprimer l'element selectionne");
    actionSupprimer->setStatusTip("Supprimer l'element selectionne");
    actionSupprimer->setShortcut(QKeySequence("Delete"));
    actionSupprimer->setIcon(QIcon(":/images/delete.png"));
    QObject::connect(actionSupprimer,SIGNAL(triggered()),this,SLOT(on_actionSupprimer_triggered()));

    actionRenommer = new QAction("&Renommer",this);
    actionRenommer->setToolTip("Renommer l'element selectionne");
    actionRenommer->setStatusTip("Renommer l'element selectionne");
    actionRenommer->setIcon(QIcon(":/images/rename.png"));
    QObject::connect(actionRenommer,SIGNAL(triggered()),this,SLOT(on_actionRenommer_triggered()));

    actionProprietes = new QAction("&Proprietes",this);
    actionProprietes->setToolTip("Proprietes de l'element selectionne");
    actionProprietes->setStatusTip("Proprietes de l'element selectionne");
    actionProprietes->setShortcut(QKeySequence("Ctrl+P"));
    actionProprietes->setIcon(QIcon(":/images/settings.png"));
    QObject::connect(actionProprietes,SIGNAL(triggered()),this,SLOT(on_actionProprietes_triggered()));


    actionApropos = new QAction("&A propos",this);
    actionApropos->setToolTip("A propos de ce projet");
    actionApropos->setStatusTip("A propos de ce projet");
    actionApropos->setShortcut(QKeySequence("F1"));
    actionApropos->setIcon(QIcon(":/images/about.png"));
    QObject::connect(actionApropos,SIGNAL(triggered()),this,SLOT(on_actionApropos_triggered()));


    actionBureau = new QAction("&Bureau",this);
    actionBureau->setToolTip("Aller au bureau");
    actionBureau->setStatusTip("Aller au bureau");
    actionBureau->setIcon(QIcon(":/images/computer.png"));
    QObject::connect(actionBureau,SIGNAL(triggered()),this,SLOT(on_actionBureau_triggered()));

    actionDocument = new QAction("&Documents",this);
    actionDocument->setToolTip("Aller aux documents");
    actionDocument->setStatusTip("Aller aux documents");
    actionDocument->setIcon(QIcon(":/images/documents.png"));
    QObject::connect(actionDocument,SIGNAL(triggered()),this,SLOT(on_actionDocument_triggered()));

    actionImages = new QAction("&Images",this);
    actionImages->setToolTip("Aller aux images");
    actionImages->setStatusTip("Aller aux images");
    actionImages->setIcon(QIcon(":/images/image.png"));
    QObject::connect(actionImages,SIGNAL(triggered()),this,SLOT(on_actionImages_triggered()));

    actionMusique = new QAction("&Musique",this);
    actionMusique->setToolTip("Aller aux musiques");
    actionMusique->setStatusTip("Aller aux musiques");
    actionMusique->setIcon(QIcon(":/images/music.png"));
    QObject::connect(actionMusique,SIGNAL(triggered()),this,SLOT(on_actionMusique_triggered()));

    actionTelechargements = new QAction("&Telechargements",this);
    actionTelechargements->setToolTip("Aller aux telechargements");
    actionTelechargements->setStatusTip("Aller aux telechargements");
    actionTelechargements->setIcon(QIcon(":/images/Download.png"));
    QObject::connect(actionTelechargements,SIGNAL(triggered()),this,SLOT(on_actionTelechargements_triggered()));

    actionVideos = new QAction("&Videos",this);
    actionVideos->setToolTip("Aller aux videos");
    actionVideos->setStatusTip("Aller aux videos");
    actionVideos->setIcon(QIcon(":/images/movies.png"));
    QObject::connect(actionVideos,SIGNAL(triggered()),this,SLOT(on_actionVideos_triggered()));


}

void MaFenetre::createMenus()
{
        toolBar = addToolBar("Fichier");

        menuFichier = menuBar()->addMenu("&Fichier");
        menuFichier->addAction(actionNouveau);
        toolBar->addAction(actionNouveau);
        menuFichier->addAction(actionNouvelleFenetre);
        toolBar->addAction(actionNouvelleFenetre);
        menuFichier->addAction(actionOuvrir);
        toolBar->addAction(actionOuvrir);
        menuFichier->addAction(actionEnregistrer);
        toolBar->addAction(actionEnregistrer);
        menuFichier->addAction(actionEnregistrerSous);
        menuFichier->addSeparator();
        menuFichier->addAction(actionQuitter);

        menuEdition = menuBar()->addMenu("&Edition");
        menuEdition->addAction(actionCouper);
        toolBar->addAction(actionCouper);
        menuEdition->addAction(actionCopier);
        toolBar->addAction(actionCopier);
        menuEdition->addAction(actionColler);
        toolBar->addAction(actionColler);
        menuEdition->addAction(actionSupprimer);
        toolBar->addAction(actionSupprimer);
        menuEdition->addAction(actionRenommer);
        toolBar->addAction(actionProprietes);

        toolBar->addWidget(label);
        toolBar->addAction(actionBureau);
        toolBar->addAction(actionDocument);
        toolBar->addAction(actionImages);
        toolBar->addAction(actionMusique);
        toolBar->addAction(actionTelechargements);
        toolBar->addAction(actionVideos);

        menuAide = menuBar()->addMenu("&Aide");
        menuAide->addAction(actionApropos);


}

void MaFenetre::on_m_bouton4_clicked()      //pour retourner au parent du dossier courant
{
    QFileInfo info = modele->fileInfo(m_tableView->rootIndex());
    QDir parent = info.dir();

    if(info.isRoot())
    {
        m_tableView->setRootIndex(filemodele->setRootPath(""));
        m_lineEdit1->setText("");
    }

    else if(parent.exists())
    {
        QString sPath = parent.absolutePath();
        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        m_lineEdit1->setText(sPath);
    }
}


void MaFenetre::on_m_bouton1_clicked()             //pour faire precedent
{
    if(pos > 0)
    {
        pos=pos-1;
        QString sPath=liste[pos];
        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        m_lineEdit1->setText(sPath);

    }
   else
    {
        m_tableView->setRootIndex(filemodele->setRootPath(""));
        m_lineEdit1->setText("");
    }


}

void MaFenetre::on_m_bouton2_clicked()           //pour faire suivant
{
    if(pos>=0 && pos < liste.count()-1)
    {
        pos=pos+1;
        QString sPath=liste[pos];
        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        m_lineEdit1->setText(sPath);

    }
}

void MaFenetre::on_actionNouveau_triggered()
{
    QModelIndex index = filemodele->setRootPath(liste[pos]);
    if(!index.isValid()) return;                                     // si l'index est invalide rien ne se passe

    QString name= QInputDialog::getText(this,"Nouveau dossier","Entrer le nom du noveau dossier");

    if(name.isEmpty()) return;                  // si on entre aucun nom rien ne se passe
    filemodele->mkdir(index,name);                 // si un nomest entre on cree un dossier avec ce nom

}

void MaFenetre::on_actionNouvelleFenetre_triggered()         // on cree ici une nouvelle fenetre
{
    MaFenetre *newFenetre = new MaFenetre();
    newFenetre->show();

}

void MaFenetre::on_actionRenommer_triggered()
{
    QModelIndex index = m_tableView->currentIndex();                                       //on recupere l'index courant
    QString name = QInputDialog::getText(this,"Nouveau nom","Entrer le noveau nom");       //on demande le nouveau nom
    QString oldPath= modele->fileInfo(index).absoluteFilePath();

    QFile file(oldPath);

    if(file.rename(liste[pos]+"/"+name))  //  liste[pos]  represent le chemin absolu du dossier dans lequel on se trouve
    {
       qDebug() << liste[pos] ;
      qDebug() <<"l'operation a marche";
      m_lineEdit1->setText(liste[pos]+"/"+name);

      actionRenommer->setEnabled(false);
    }
    else
    {
        QMessageBox::about(this, tr("Erreur de Renommage"),
                 tr("Le renommage a echoue !!!"));                        // si l'element n'est pas renomme
        qDebug() <<"l'operation a echoue";
        qDebug() << liste[pos]+"/"+name ;
    }


}

void MaFenetre::on_actionSupprimer_triggered()
{
    table = m_tableView->SelectedIndexes();
    int rep=QMessageBox::warning(this,"Supprimer...","Voulez-vous vraiment supprimer le(s) element(s) selectionne(s) ?",
                                        QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

    if(rep==QMessageBox::Yes)
    {
        if(table.count()>0)
        {
            for(i=0;i<table.count();i++)
            {
                if(filemodele->fileInfo(table[i]).isDir())
                {
                    filemodele->remove(table[i]);
                }
                else if(filemodele->fileInfo(table[i]).isFile())
                {
                    filemodele->remove(table[i]);
                }
            }
            table.clear();                              // on supprime tout le contenu de table
            actionSupprimer->setEnabled(false);        // desactiver l'action actionSupprimer
         }

     }

}

void MaFenetre::on_actionCopier_triggered()
{
         ind2=ind1;                                              // on garde l'index de l'element copie

        filePath=modele->fileInfo(ind2).absoluteFilePath();
        fileName=modele->fileInfo(ind2).fileName();

        for(i=0;i<tableau.count();i++)
        {
            tableau[i]->actionColler->setEnabled(true);                     // rendre l'action "actionColler" active
            tableau[i]->actionCouper->setEnabled(false);
        }



        estCopie=true;                                   //estCopie recoit la valeur vraie

}

void MaFenetre::on_actionColler_triggered()
{

           QFile file(filePath);
           qDebug() <<filePath;
           qDebug() << liste[pos];
           qDebug() << liste[pos]+"/"+fileName;    //liste[pos] contient le chemin absolu du dossier dans lequel on se trouve (le dossier parent courant)

           file.copy(liste[pos]+"/"+fileName);

           qDebug() << liste[pos];

  if(!estCopie)
   {
       filemodele ->remove(ind2);      // estCopie vaut false donc c'est la coupure qui a ete effectue .il faul donc supprimer l'elemnt dans l'ancienne emplacement repere par l'index ind2
   }

   actionColler->setEnabled(false);   // desactiver l'action coller
   actionCopier->setEnabled(false);   // desactiver l'action copier
   actionCouper->setEnabled(false);

}

void MaFenetre::on_actionCouper_triggered()
{
        ind2 = ind1;  // on garde l'index du fichier qu'on veux couper au cas ou l'utilisateur cliquerait sur autre chose avant de coller le fichier
        filePath=modele->fileInfo(ind2).absoluteFilePath();
        fileName=modele->fileInfo(ind2).fileName();


        for(i=0;i<tableau.count();i++)
        {
            tableau[i]->actionColler->setEnabled(true);                     // rendre l'action "actionColler" active
            tableau[i]->actionCopier->setEnabled(false);
        }


    estCopie = false;  //estCopie recoit la valeur fausse

}


void MaFenetre::on_actionOuvrirFichier_triggered()
{
    QModelIndex index = m_tableView->currentIndex();
    QString sPath = modele->fileInfo(index).absoluteFilePath();
    if(modele->fileInfo(index).isFile())
    {
        const char* spath = sPath.toStdString().c_str();
        ouvrir_fichier(spath);
    }
    else
    {
        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        if(pos<(liste.count()-1))
        {
            for(i=pos+1;i<= (liste.count()-1);i++)
            {
                liste.removeAt(i);                             // supprimer l'element du tableau liste dont l'indice est i
            }
        }

        liste.append(sPath);
        pos=pos+1;
    }

}


void MaFenetre::on_actionProprietes_triggered()
{
    secondeFenetre = new QDialog(this);                            // on cree une QDialog qui represente la fenetre secondaire
    secondeFenetre->resize(600,250);                                 // on fixe les dmensions de depart mais elle peuvent etres modifiees par la suite
    m_flayout = new QFormLayout;
    QModelIndex index = m_tableView->currentIndex();
    QString cheminFichier = modele->fileInfo(index).absoluteFilePath();
    QString nomFichier = modele->fileInfo(index).fileName();
    int tailleFichier = modele->fileInfo(index).size();
    QString tailleF;
    QDateTime temps1 = modele->fileInfo(index).birthTime();
    QDateTime temps2 = modele->fileInfo(index).lastModified();
    QDateTime temps3 = modele->fileInfo(index).lastRead();
    tailleF.setNum(tailleFichier); // no transforme la taille du fichier en chaine de caracteres
    QString typeFichier;
    if(modele->fileInfo(index).isFile())
    {
      typeFichier="fichier";
    }
    else if(modele->fileInfo(index).isDir())
    {
        typeFichier="dossier";
    }
    QLineEdit *nom = new QLineEdit;
    QLineEdit *chemin = new QLineEdit;
    QLineEdit *taille = new QLineEdit;
    QLineEdit *type = new QLineEdit;
    QDateTimeEdit *time1= new QDateTimeEdit;
    QDateTimeEdit *time2= new QDateTimeEdit;
    QDateTimeEdit *time3= new QDateTimeEdit;
    nom->setText(nomFichier);
    nom->setReadOnly(true);                     // on peut uniquement lire les proprietes on ne peut pas les modifiees
    chemin->setText(cheminFichier);
    chemin->setReadOnly(true);
    type->setText(typeFichier);
    type->setReadOnly(true);
    taille->setText(tailleF+" octets");
    taille->setReadOnly(true);
    time1->setDateTime(temps1);
    time1->setReadOnly(true);
    time2->setDateTime(temps2);
    time2->setReadOnly(true);
    time3->setDateTime(temps3);
    time3->setReadOnly(true);

    m_flayout->addRow("&Nom: ", nom);
    m_flayout->addRow("&Chemin absolu: ", chemin);
    m_flayout->addRow("&Date de creation: ", time1);
    m_flayout->addRow("&Date de la derniere modification: ", time2);
    m_flayout->addRow("&Date de la derniere lecture: ", time3);

    if(modele->fileInfo(index).isFile())                                    // cette ligne ne s'affiche que s'il s'agit d'un fichier et nous informe sur la taille de celui ci
    {
       m_flayout->addRow("&Taille: ", taille);
    }

    m_flayout->addRow("&Type: ", type);                                    // ajoute une ligne au formulaire

    secondeFenetre->setLayout(m_flayout);
    secondeFenetre->exec();

}


void MaFenetre::on_actionBureau_triggered()
{

    char spath [MAX_PATH];                                         // on cree une chainr de ceracteres
    HRESULT result = SHGetFolderPathA(NULL,CSIDL_DESKTOP,NULL,SHGFP_TYPE_CURRENT,spath);                 // cette fonction retourne par líntermediaire de spath e chemin absolu du dossier mesdocuments de l'ordinateur

    if(result == S_OK)
    {
        QString sPath = (QString)spath;

        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        if(pos<(liste.count()-1))
        {
            for(i=pos+1;i<= (liste.count()-1);i++)
            {
                liste.removeAt(i);
            }
        }
        liste.append(sPath);      // on ajoute ce chemin dans le tableau liste
        pos=pos+1;                   // pos s'incremente
        m_lineEdit1->setText(sPath);

    }

}

void MaFenetre::on_actionDocument_triggered()
{

    char spath [MAX_PATH];                                         // on cree une chainr de ceracteres
    HRESULT result = SHGetFolderPathA(NULL,CSIDL_MYDOCUMENTS,NULL,SHGFP_TYPE_CURRENT,spath);                 // cette fonction retourne par líntermediaire de spath e chemin absolu du dossier mesdocuments de l'ordinateur

    if(result == S_OK)
    {
        QString sPath = (QString)spath;

        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        if(pos<(liste.count()-1))
        {
            for(i=pos+1;i<= (liste.count()-1);i++)
            {
                liste.removeAt(i);
            }
        }
        liste.append(sPath);      // on ajoute ce chemin dans le tableau liste
        pos=pos+1;                   // pos s'incremente
        m_lineEdit1->setText(sPath);

    }

}

void MaFenetre::on_actionImages_triggered()
{

    char spath [MAX_PATH];                                         // on cree une chainr de ceracteres
    HRESULT result = SHGetFolderPathA(NULL,CSIDL_MYPICTURES,NULL,SHGFP_TYPE_CURRENT,spath);                 // cette fonction retourne par líntermediaire de spath e chemin absolu du dossier mesdocuments de l'ordinateur

    if(result == S_OK)
    {
        QString sPath = (QString)spath;

        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        if(pos<(liste.count()-1))
        {
            for(i=pos+1;i<= (liste.count()-1);i++)
            {
                liste.removeAt(i);
            }
        }
        liste.append(sPath);      // on ajoute ce chemin dans le tableau liste
        pos=pos+1;                   // pos s'incremente
        m_lineEdit1->setText(sPath);

    }

}

void MaFenetre::on_actionMusique_triggered()
{

    char spath [MAX_PATH];                                         // on cree une chainr de ceracteres
    HRESULT result = SHGetFolderPathA(NULL,CSIDL_MYMUSIC,NULL,SHGFP_TYPE_CURRENT,spath);                 // cette fonction retourne par líntermediaire de spath e chemin absolu du dossier mesdocuments de l'ordinateur

    if(result == S_OK)
    {
        QString sPath = (QString)spath;

        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        if(pos<(liste.count()-1))
        {
            for(i=pos+1;i<= (liste.count()-1);i++)
            {
                liste.removeAt(i);
            }
        }
        liste.append(sPath);      // on ajoute ce chemin dans le tableau liste
        pos=pos+1;                   // pos s'incremente
        m_lineEdit1->setText(sPath);

    }

}

void MaFenetre::on_actionTelechargements_triggered()
{

    char spath [MAX_PATH];                                         // on cree une chainr de ceracteres
    HRESULT result = SHGetFolderPathA(NULL,CSIDL_PROFILE,NULL,SHGFP_TYPE_CURRENT,spath);                 // cette fonction retourne par líntermediaire de spath e chemin absolu du dossier mesdocuments de l'ordinateur

    if(result == S_OK)
    {
        QString sPath = (QString)spath;
        sPath = sPath+"/Downloads" ;

        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        if(pos<(liste.count()-1))
        {
            for(i=pos+1;i<= (liste.count()-1);i++)
            {
                liste.removeAt(i);
            }
        }
        liste.append(sPath);      // on ajoute ce chemin dans le tableau liste
        pos=pos+1;                   // pos s'incremente
        m_lineEdit1->setText(sPath);

    }

}

void MaFenetre::on_actionVideos_triggered()
{

    char spath [MAX_PATH];                                         // on cree une chainr de ceracteres
    HRESULT result = SHGetFolderPathA(NULL,CSIDL_MYVIDEO,NULL,SHGFP_TYPE_CURRENT,spath);                 // cette fonction retourne par líntermediaire de spath e chemin absolu du dossier mesdocuments de l'ordinateur

    if(result == S_OK)
    {
        QString sPath = (QString)spath;

        m_tableView->setRootIndex(filemodele->setRootPath(sPath));
        if(pos<(liste.count()-1))
        {
            for(i=pos+1;i<= (liste.count()-1);i++)
            {
                liste.removeAt(i);
            }
        }
        liste.append(sPath);      // on ajoute ce chemin dans le tableau liste
        pos=pos+1;                   // pos s'incremente
        m_lineEdit1->setText(sPath);

    }

}


MaFenetre::~MaFenetre()
{
}

