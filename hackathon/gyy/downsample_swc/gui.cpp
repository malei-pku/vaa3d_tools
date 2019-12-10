#include "gui.h"

DownsampleDialog :: DownsampleDialog(QWidget *parent): QDialog(parent)
{
    res_input = new QSpinBox();
    res_input -> setRange(1, 7);
    res_input -> setValue(1);

    res_output = new QSpinBox();
    res_output -> setRange(1, 7);
    res_output -> setValue(6);
    connect(res_input, SIGNAL(valueChanged(int)), res_input, SLOT(setValue(int)));
    connect(res_output, SIGNAL(valueChanged(int)), res_output, SLOT(setValue(int)));

    input_file_Btn = new QPushButton(tr("input file"));
    file_edit = new QLineEdit(tr("Select a file !"));
    hbox1 = new QHBoxLayout();
    hbox1 -> addWidget(input_file_Btn);
    hbox1 -> addWidget(file_edit);
    connect(input_file_Btn, SIGNAL(clicked(bool)), this, SLOT(selectFiles()));

    input_folder_Btn = new QPushButton(tr("output file"));
    folder_edit = new QLineEdit(tr("Select a folder !"));
    hbox2 = new QHBoxLayout();
    hbox2 -> addWidget(input_folder_Btn);
    hbox2 -> addWidget(folder_edit);
    connect(input_folder_Btn, SIGNAL(clicked(bool)), this, SLOT(selectFiles()));

    saveBtn = new QPushButton(tr("save files"));
    files_save = new QLineEdit(tr("..."));
    hbox_save = new QHBoxLayout();
    hbox_save -> addWidget(saveBtn);
    hbox_save -> addWidget(files_save);
    connect(saveBtn, SIGNAL(clicked(bool)), this, SLOT(saveFiles()));

    ok = new QPushButton(tr("ok"));
    ok -> setDefault(true);
    cancel = new QPushButton(tr("cancel"));
    cancel -> setDefault(false);
    hbox3 = new QHBoxLayout();
    hbox3 -> addWidget(ok);
    hbox3 -> addWidget(cancel);
    connect(ok, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

    gridLayout = new QGridLayout();
    gridLayout -> addWidget(new QLabel("the highest resolution is number 1 !"), 0, 0);
    gridLayout -> addWidget(new QLabel("the resolution of the imported files"), 1, 0);
    gridLayout -> addWidget(res_input, 1, 1);
    gridLayout -> addWidget(new QLabel("the resolution of the exported files"), 2, 0);
    gridLayout -> addWidget(res_output, 2, 1);

    gridLayout -> addLayout(hbox1, 3, 0);
    gridLayout -> addLayout(hbox2, 4, 0);
    gridLayout -> addLayout(hbox_save, 5, 0);
    gridLayout -> addLayout(hbox3, 6, 0);

    this -> setLayout(gridLayout);
    this -> setWindowTitle("downSample files");
}

void DownsampleDialog:: selectFiles()
{
    QPushButton *button = (QPushButton *) sender();
    if(button == input_file_Btn)
    {
        file = QFileDialog :: getOpenFileName(NULL,tr("select a file"),"D:\\","files(*.eswc *.swc *.apo)");
        file_edit->setText(file);
        qDebug()<<__LINE__<<" : "<<file.toStdString().c_str();
        if(file.isEmpty())
        {
            file_edit->setText("Select a file !");
            v3d_msg("File is empty ! Input again !");
        }
    }
    else if(button == input_folder_Btn)
    {
        files = QFileDialog::getExistingDirectory(NULL, tr("select a folder"),"D:\\");
        folder_edit->setText(files);
        if(files.isEmpty())
        {
            folder_edit->setText("Select a folder !");
            v3d_msg("Folder is empty ! Input again !");
        }
    }
}

void DownsampleDialog:: saveFiles()
{
    QPushButton *button1 = (QPushButton *) sender();
    if(button1 == saveBtn)
    {
        if(file != "" && files == "")
        {
            QString file_tem = nameSaveFile(file);
            fileSave = QFileDialog :: getSaveFileName(0, tr("Select a file to save"), file_tem, tr("(.*).eswc.swc.apo"));
            files_save->setText(fileSave);
            if(fileSave.isEmpty())
            {
                files_save->setText("...");
                v3d_msg("Please input a file to save again !");
            }
        }
        else if(file == "" && files != "")
        {
            folderSave = QFileDialog :: getExistingDirectory(0, tr("Select a folder to save files"), files);
            files_save->setText(folderSave);
            if(folderSave.isEmpty())
            {
                files_save->setText("...");
                v3d_msg("Please input a folder to save again !");
            }
        }
    }
}

void DownsampleDialog:: downsampleEswc(const QString file, QString saveName, double down)
{
    qDebug()<<__LINE__<<" : "<<file.toStdString().c_str();
    NeuronTree nt = readSWC_file(file);
    qDebug()<<"nt size : "<<nt.listNeuron.size();
    for(V3DLONG i = 0; i < nt.listNeuron.size(); i ++)
    {
        nt.listNeuron[i].x /= down;
        nt.listNeuron[i].y /= down;
        nt.listNeuron[i].z /= down;
    }
    if(file.endsWith(".eswc"))
        writeESWC_file(saveName, nt);
    else if(file.endsWith(".swc"))
        writeSWC_file(saveName, nt);
}

void DownsampleDialog:: downsampleApo(const QString file, QString saveName, double down)
{
    QList<CellAPO> marker_apo = readAPO_file(file);
    qDebug()<<"apo size : "<<marker_apo.size();
    for(V3DLONG i = 0; i < marker_apo.size(); i ++)
    {
        marker_apo[i].x /= down;
        marker_apo[i].y /= down;
        marker_apo[i].z /= down;
    }
    writeAPO_file(saveName, marker_apo);
}

QString DownsampleDialog:: nameSaveFile(const QString file)
{
    QString file_tem_save;
    if(file.endsWith(".eswc"))
        file_tem_save = file + "_downsample.eswc";
    else if(file.endsWith(".swc"))
        file_tem_save = file + "_downsample.swc";
    else if(file.endsWith(".apo"))
        file_tem_save = file + "_downsample.apo";
    return file_tem_save;
}














