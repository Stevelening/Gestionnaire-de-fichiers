#ifndef MAFENETRE_H
#define MAFENETRE_H

// on inclus toutes les classes dont on aura besoin pour le bon fonctionnement de notre programme(qui interviennent dans ce programme)

#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTreeView>
#include <QTableView>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QDialog>
#include <QAbstractItemView>
#include <QList>
#include <QSplitter>
#include <QModelIndex>
#include <QContextMenuEvent>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <QStatusBar>
#include <QLabel>
#include <windows.h>
#include <ShellApi.h>
#include <shlobj.h>
#include <shlwapi.h>

#include "TableView.h"



class MaFenetre : public QMainWindow
{

    Q_OBJECT                                                   // indispensable(ou obligatoire) pour creer des signaux et des slots


    public:

        MaFenetre(QWidget *parent = nullptr);         // constructeur de MaFenetre
       ~MaFenetre();

       void ouvrir_fichier(const char* FilePath);                // methode qui ouvre des fichiers



    private:

       int pos = -1, i;                                  // pos repere la position dans le tableau liste et i eat un compteur entier
       int p=0;
       QList<QString> liste;                      // on cree un tableau dynamique pour contenir les index des elements sur lesquels on double-clic
       static QModelIndex ind1,ind2;                      // deux index pour garder l'index d'un element lorsque celui ci mest selectionne
       static QString fileName;                  // "fileName" contient le nom du fichier courant et "filePath" contient les chemins
       static QString filePath;                            // on declare ces variables static parcequ'on a besoin dý acceder dans toutes les instances de la classe MaFenetre
       QModelIndexList table;                      // on cree une liste d'index
       bool estCopie;                                      // retourne true si c'est la copie et false si c'est la coupure(pour distinguer "couper" de "copier")
       static QList<MaFenetre*> tableau;                      // permet de contenir les pointeurs de chaque instances de la classe ma fenetre qu'on cree

       QPushButton *m_bouton1;                              // on cree des boutons
       QPushButton *m_bouton2;
       QPushButton *m_bouton3;
       QPushButton *m_bouton4;
       QPushButton *m_bouton5;

       QLineEdit *m_lineEdit1;                                // on cree une zone de texte lune pour afficher le chemin absolue de l'element selectionne
       QLineEdit *m_lineEdit2;                                // on cree une zone de texte lune pour saisir l'element a rechercher

       QVBoxLayout *m_vlayout;                     // un conteneur vertical
       QHBoxLayout *m_hlayout1;                     // 03 conteneurs horizontaux
       QHBoxLayout *m_hlayout2;
       QHBoxLayout *m_hlayout3;
       QFormLayout *m_flayout;                        // un comteneur en formulaire pour les proprietes
       QVBoxLayout *m_vlayout1;
       QHBoxLayout *m_hlayout11;


       QTreeView *m_treeView;             // on cree deux vues une en abre et l'autre en table
       TableView *m_tableView;
       TableView *m_tableView1;

       QSplitter *split1;                  //on cree des separateurs
       QSplitter *split2;

       QFileSystemModel *modele;                   //on cree deux modeles
       QFileSystemModel *filemodele;
       QStandardItemModel *model;

       void createActions();                        // on cree des actions et des menus via deux methodes
       void createMenus();

       QAction *actionNouveau;                  //on cree les differentes actions
       QAction *actionNouvelleFenetre;
       QAction *actionOuvrir;
       QAction *actionOuvrirFichier;
       QAction *actionEnregistrer;
       QAction *actionEnregistrerSous;
       QAction *actionQuitter;
       QAction *actionCouper;
       QAction *actionCopier;
       QAction *actionColler;
       QAction *actionSupprimer;
       QAction *actionRenommer;
       QAction *actionProprietes;
       QAction *actionApropos;
       QAction *actionBureau;
       QAction *actionDocument;
       QAction *actionImages;
       QAction *actionMusique;
       QAction *actionTelechargements;
       QAction *actionVideos;
       QMenu *menuFichier;                           // on cree les differents menus
       QMenu *menuEdition;
       QMenu *menuAide;
       QToolBar *toolBar;                           // c'estla barre d'outils

       QDialog *secondeFenetre;                   //on cree une fenetre secondaire pour afficher les proprietes
       QDialog *fenetreRecherche;
       QLabel *label;
       QLabel *label1;



    public slots:
                                                                   // on cree les differents slots
       void on_actionQuitter_triggered();

       void on_m_treeView_activated(const QModelIndex &index);

       void on_m_tableView_clicked(const QModelIndex &index);

       void on_m_bouton5_clicked();

       void on_m_treeView_clicked(const QModelIndex &index);

       void on_m_tableView_activated(const QModelIndex &index);

       void on_m_tableView1_activated(const QModelIndex &index);

       void on_actionOuvrir_triggered();

       void on_actionEnregistrer_triggered();

       void on_m_bouton3_clicked();

       void on_actionApropos_triggered();

       void on_m_bouton4_clicked();

       void on_m_bouton1_clicked();

       void on_m_bouton2_clicked();

       void on_actionNouveau_triggered();

       void on_actionNouvelleFenetre_triggered();

       void on_actionRenommer_triggered();

       void on_actionSupprimer_triggered();

       void on_actionCopier_triggered();

       void on_actionCouper_triggered();

       void on_actionColler_triggered();

       void on_actionOuvrirFichier_triggered();

       void on_actionProprietes_triggered();

       void on_clickDroit_activated(QContextMenuEvent *event);

       void on_actionBureau_triggered();

       void on_actionDocument_triggered();

       void on_actionImages_triggered();

       void on_actionMusique_triggered();

       void on_actionTelechargements_triggered();

       void on_actionVideos_triggered();

       void recherche(QFileInfo fileInfo,QStandardItemModel *model,QStringList filtre);



};

#endif // MAFENETRE_H
