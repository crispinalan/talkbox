/***************************************************************************
 *   Author Alan Crispin                 *
 *   crispinalan@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation.                                  *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//info
#include <QDebug>
#include <QMessageBox>



//audio
#include <QSound>
#include <QSoundEffect>
#include <QEventLoop>

#include <QDesktopServices>

//file
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>


#include "speechengine.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override; 


private slots:

    //menu actions

    void on_actionNew_triggered(); //new
    void on_actionOpen_triggered(); //open
    void on_actionSave_triggered(); //save
    void on_actionSaveAs_triggered(); //save
    void on_actionExit_triggered(); //exit

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();


    void on_actionAbout_triggered(); //about

    //button slot
    void on_pushButtonSpeak_clicked();

	
private:
    Ui::MainWindow *ui;

     QString theFileName;

     //QString text;

    void setApplicationFontSize(int fontSize);

   //speaking pointers
    //Todo: update to QScopedPointer
    std::unique_ptr<QSoundEffect> m_soundEffect;
    std::unique_ptr<QEventLoop> m_eventLoop;
    
    SpeechEngine *speech;
    void updateEventLoop();

    void speakText();

    bool directoryExists(QString path);
    bool fileExists(QString path);


};

#endif // MAINWINDOW_H
