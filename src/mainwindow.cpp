/***************************************************************************
 *   Author Alan Crispin                  *
 *   crispinalan@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation.                                 *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

  ui->setupUi(this);
  setWindowTitle("TalkBox");
  QMainWindow::centralWidget()->layout()->setContentsMargins(2, 2, 2, 2);

  QMainWindow::resize(800,600);
  setApplicationFontSize(18);
  setWindowIcon(QIcon(":/icons/talkbox.png"));

  //Speaking
  m_soundEffect = std::make_unique<QSoundEffect>();
  m_soundEffect->setLoopCount(1);
  m_eventLoop = std::make_unique<QEventLoop>();
  connect(m_soundEffect.get(), SIGNAL(playingChanged()), m_eventLoop.get(), SLOT(quit()));

  //set up speech engine
  speech = new SpeechEngine();
  //text ="hello";

}

MainWindow::~MainWindow()
{
    delete ui;
}

//--------------------------------------------------------------------
//menu items
//--------------------------------------------------------------------
void MainWindow::on_actionNew_triggered()
{
    //New Action
    theFileName="";
    ui->textEdit->setPlainText("");
}


void MainWindow::on_actionOpen_triggered()
{
        //qDebug()<<"open file\n";

       //open file action
    QString fileName = QFileDialog::getOpenFileName(this,"Open File");
    if (!fileName.isEmpty())
    {
        QFile openFile(fileName);
        if (openFile.open(QFile::ReadOnly | QFile::Text))
        {
            this->theFileName =fileName; //capture file name
            QTextStream inStream(&openFile); //open text stream
            QString text = inStream.readAll(); //read entire content of stream

            openFile.close(); //close the stream
            ui->textEdit->setPlainText(text);//set textEdit text
        }
    }

}

void MainWindow::on_actionSave_triggered()
{
     //Save file action
    if (this->theFileName =="")//Test for blank file name
    {
        on_actionSaveAs_triggered();
    }
    QFile saveFile(theFileName);
    if (saveFile.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream outStream(&saveFile);
        outStream << ui->textEdit->toPlainText();
        saveFile.flush();
        saveFile.close();
    }

}

void MainWindow::on_actionSaveAs_triggered()
{
    //saveAs file action
    QString fileName = QFileDialog::getSaveFileName(this,"Save As File");
    if (!fileName.isEmpty())
    {
        this->theFileName =fileName;
        on_actionSave_triggered();
    }
}


void MainWindow::on_actionExit_triggered()
{

    QApplication::quit();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
        //qDebug()<<"close event\n";
}

void MainWindow::on_actionUndo_triggered()
{
    //Undo action
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    //Redo action
    ui->textEdit->redo();
}

void MainWindow::on_actionCut_triggered()
{
    //Cut action
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    //Copy action
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    //Paste action
    ui->textEdit->paste();
}

void MainWindow::updateEventLoop(){

    if(fileExists(QApplication::applicationDirPath()+"/sounds/bleep.wav")) {
        qDebug()<<"bleep found\n";
        QString _bleep= QApplication::applicationDirPath()+"/sounds/bleep.wav";
        m_soundEffect->setSource(QUrl::fromLocalFile(_bleep));
        m_soundEffect->play();
        m_eventLoop->exec();
    }
    else{
        qDebug()<<"bleep not found\n";
        m_soundEffect->setSource(QUrl());
        m_soundEffect->play();
        m_eventLoop->exec();
    }
}

//-----------------------------------------------------------------------------

void MainWindow::on_pushButtonSpeak_clicked()
{
	//qDebug()<<"speak\n";
    updateEventLoop();
    speakText();
}

//-------------------------------------------------

void MainWindow::speakText() {

     //qDebug()<<"speakText reached ...\n";

    QString talkStr= ui->textEdit->toPlainText();

     //Check if diphone directory exists
    QString dictPath =QApplication::applicationDirPath()+"/diphone/";

    if(!directoryExists(dictPath)) {
		QString str ="Speak diphone directory does not exist."
		  "\nInstall diphones by unzipping diphone.tar to application path.";
		QMessageBox::information(this, "Speak Diphones Missing", str);
		return;
	}


	speech->generateTalkOut(talkStr);

	if(fileExists("/tmp/talkout.wav")) {
	m_soundEffect->setSource(QUrl::fromLocalFile("/tmp/talkout.wav"));
	m_soundEffect->play();
	m_eventLoop->exec();
	}
	else {
		//to do..
	}
}


//help menu
void MainWindow::on_actionAbout_triggered() {
  QString about_text = "TalkBox is a diphone speech synthesiser for reading plain text."
                       "\nBuilt with Qt 5.15"
                       "\nAlan Crispin (2023)";
  QMessageBox::information(this, "TalkBox (Qt) v0.1.1",
                           about_text);
}




void MainWindow::setApplicationFontSize(int fontSize) {

  QFont appfont = QApplication::font();
  appfont.setPixelSize(fontSize); //DPI
  QApplication::setFont(appfont); 
}







bool MainWindow::directoryExists(QString path)
{
	const QFileInfo outputDir(path);
    if ((!outputDir.exists()) || (!outputDir.isDir()) || (!outputDir.isReadable())) {
        //qWarning() << "output directory does not exist, is not a directory, or is not readable"
                   //<< outputDir.absoluteFilePath();
        return false;
    }
    else {
		return true;
	}
}

bool MainWindow::fileExists(QString path)
{
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and not directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

