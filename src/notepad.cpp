#include "notepad.h"
#include "ui_notepad.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    MainWindow* newWindow = new MainWindow();
    newWindow->show();
    currentFile.clear();
    ui->textEdit->setText(QString());
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Opening file");
    if(filename.isEmpty()) return;

    QFile file(filename);
    currentFile = filename;
    if(!file.exists()){
        QMessageBox::warning(this, "Warning", "File does not exist!");
        return;
    }

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "File is not readable!");
        return;
    }
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    if(filename.isEmpty()) return;

    QFile file(filename);
    currentFile = filename;
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "File is not writable!");
        return;
    }
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    QPrintDialog pdiag(&printer, this);
    if(pdiag.exec() == QDialog::Rejected){
        return;
    }
    ui->textEdit->print(&printer);
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionSave_triggered()
{
    if(currentFile.isEmpty()){
        on_actionSave_as_triggered();
        return;
    }
    QFile file(currentFile);
    if(!file.exists()){
        QMessageBox::warning(this, "Warning", "File does not exist!");
        on_actionSave_as_triggered();
        return;
    }

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "File is not writable!");
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}
