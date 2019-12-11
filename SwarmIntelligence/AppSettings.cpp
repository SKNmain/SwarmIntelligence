#include "AppSettings.h"
#include "ui_AppSettingsUI.h"
#include <QColorDialog>
#include <QDebug>


AppSettings::AppSettings(QWidget* parent) :
   QDialog(parent), ui(new Ui::AppSettings)
{
   ui->setupUi(this);
   setWindowFlag(Qt::WindowContextHelpButtonHint, false);

   //this->ui->radio
   this->settings.sync();

   //spin boxes
   setStartingValue(MARKER_SIZE_TAG, 10, this->ui->spinBox_markerSize);
   setStartingValue(MAZE_WIDTH_TAG, 10, this->ui->spinBox_mazeWidth);
   setStartingValue(MAZE_HEIGHT_TAG, 10, this->ui->spinBox_mazeHeight);
   setStartingValue(PATH_SIZE_TAG, 3, this->ui->spinBox_pathSize);
   setStartingValue(TILE_SIZE_TAG, 5, this->ui->spinBox_tileSize);
   setStartingValue(ANIMATION_TIME_TAG, 100, this->ui->spinBox_animationTime);
   setStartingValue(ANT_SIZE_TAG, 5, this->ui->spinBox_antSize);
   setStartingValue(ANT_ANIMATION_TIME_TAG, 50, this->ui->spinBox_antsAnimationTime);
   setStartingValue(ANT_CONST_NUMBER_TAG, 5, this->ui->spinBox_constNumberOfAnts);
   //checkboxes
   setStartingValue(ANIMATION_ENABLED_TAG, true, this->ui->checkBox_animationEnabled);
   setStartingValue(VISUALIZE_TAG, true, this->ui->checkBox_visualize);
   setStartingValue(ANT_CONST_NUMBER_ENABLED_TAG, true, this->ui->checkBox_isConstNumberOfAnts);

   //colors
   setStartingValue(ANT_COLOR_TAG, Qt::green);


   if(false == this->isAnimationEnabled())
   {
      this->ui->spinBox_animationTime->setEnabled(false);
   }
   if(false == this->isVisualize())
   {
      this->ui->spinBox_animationTime->setEnabled(false);
      this->ui->checkBox_animationEnabled->setEnabled(false);
   }

   switch(startingMazeStatus())
   {
   case LOAD_FROM_FILE: this->ui->radioButton_loadMazeFromFileOnStart->setChecked(true); break;
   case GENERATE_NEW: this->ui->radioButton_generateMazeOnStart->setChecked(true); break;
   case NO_STARTING: this->ui->radioButton_noStartingMaze->setChecked(true); break;
   }

   connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

AppSettings::~AppSettings()
{
   this->settings.sync();
   delete ui;
}


void AppSettings::setStartingValue(const QString& optName, int val, QSpinBox* widget)
{
   if(false == this->settings.contains(optName))
   {
      this->settings.setValue(optName, val);
   }
   widget->setValue(this->settings.value(optName).toInt());
}

void AppSettings::setStartingValue(const QString& optName, bool val, QCheckBox* widget)
{
   if(false == this->settings.contains(optName))
   {
      this->settings.setValue(optName, val);
   }
   widget->setCheckState(this->settings.value(optName).toBool() == true ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

void AppSettings::setStartingValue(const QString& optName, const QColor& color)
{
   this->settings.setValue(optName, color);
}

int AppSettings::getMazeWidth() const
{
   return this->settings.value(MAZE_WIDTH_TAG).toInt();
}

int AppSettings::getMazeHeight() const
{
   return this->settings.value(MAZE_HEIGHT_TAG).toInt();
}

int AppSettings::getMarkerSize() const
{
   return this->settings.value(MARKER_SIZE_TAG).toInt();
}

int AppSettings::getPathSize() const
{
   return this->settings.value(PATH_SIZE_TAG).toInt();
}

int AppSettings::getTileSize() const
{
   return this->settings.value(TILE_SIZE_TAG).toInt();
}

int AppSettings::getAnimationTime() const
{
   return this->settings.value(ANIMATION_TIME_TAG).toInt();
}

int AppSettings::getAntSize() const
{
   return this->settings.value(ANT_SIZE_TAG).toInt();
}

int AppSettings::getConstNumberOfAntsSize() const
{
   return this->settings.value(ANT_CONST_NUMBER_TAG).toInt();
}

int AppSettings::getAntsAnimationTime() const
{
   return this->settings.value(ANT_ANIMATION_TIME_TAG).toInt();
}

AppSettings::StartingMaze AppSettings::startingMazeStatus() const
{
   return this->settings.value(STARTING_MAZE_TAG, QVariant::fromValue(StartingMaze::NO_STARTING)).value<StartingMaze>();
}

QString AppSettings::pathToMaze() const
{
   return this->settings.value(PATH_TO_LAST_MAZE_FILE_TAG, "").toString();
}

void AppSettings::setLastMazePath(const QString& path)
{
   this->settings.setValue(PATH_TO_LAST_MAZE_FILE_TAG, path);
}

void AppSettings::setStartingMazeStatus(StartingMaze startingMaze)
{
   switch(startingMazeStatus())
   {
   case LOAD_FROM_FILE: on_radioButton_loadMazeFromFileOnStart_clicked(); break;
   case GENERATE_NEW: on_radioButton_generateMazeOnStart_clicked(); break;
   case NO_STARTING: on_radioButton_noStartingMaze_clicked(); break;
   }
}

bool AppSettings::isConstNumberOfAntsEnabled() const
{
   return this->settings.value(ANT_CONST_NUMBER_ENABLED_TAG).toBool();
}

void AppSettings::setAntsAnimationTime(int val)
{
   on_spinBox_antsAnimationTime_valueChanged(val);
}

void AppSettings::setNumberOfAnts(int val)
{
   on_spinBox_constNumberOfAnts_valueChanged(val);
}

void AppSettings::setConstNumberOfAnts(bool val)
{
   on_checkBox_isConstNumberOfAnts_stateChanged(val);
}

void AppSettings::setAntSize(int val)
{
   on_spinBox_antSize_valueChanged(val);
}

bool AppSettings::isAnimationEnabled() const
{
   return this->settings.value(ANIMATION_ENABLED_TAG).toBool();
}

bool AppSettings::isVisualize() const
{
   return this->settings.value(VISUALIZE_TAG).toBool();
}

QColor AppSettings::getAntsColor() const
{
   return this->settings.value(ANT_COLOR_TAG).value<QColor>();
}

void AppSettings::setAntColor(const QColor& color)
{
   this->settings.setValue(ANT_COLOR_TAG, color.name());
}

void AppSettings::setVisualizeEnabled(bool val)
{
   on_checkBox_visualize_stateChanged(val);
}

void AppSettings::setAnimationTime(int val)
{
   on_spinBox_animationTime_valueChanged(val);
}

void AppSettings::setAnimationEnabled(bool val)
{
   on_checkBox_animationEnabled_stateChanged(val);
}

void AppSettings::setMazeWidth(int val)
{
   on_spinBox_mazeWidth_valueChanged(val);
}

void AppSettings::setMazeHeight(int val)
{
   on_spinBox_mazeHeight_valueChanged(val);
}

void AppSettings::setMarkerSize(int val)
{
   on_spinBox_markerSize_valueChanged(val);
}

void AppSettings::setPathSize(int val)
{
   on_spinBox_pathSize_valueChanged(val);
}

void AppSettings::setTileSize(int val)
{
   on_spinBox_tileSize_valueChanged(val);
}

void AppSettings::on_spinBox_markerSize_valueChanged(int val)
{
   this->settings.setValue(MARKER_SIZE_TAG, val);
}

void AppSettings::on_spinBox_pathSize_valueChanged(int val)
{
   this->settings.setValue(PATH_SIZE_TAG, val);
}

void AppSettings::on_spinBox_tileSize_valueChanged(int val)
{
   this->settings.setValue(TILE_SIZE_TAG, val);
}

void AppSettings::on_spinBox_mazeHeight_valueChanged(int val)
{
   this->settings.setValue(MAZE_HEIGHT_TAG, val);
}

void AppSettings::on_spinBox_mazeWidth_valueChanged(int val)
{
   this->settings.setValue(MAZE_WIDTH_TAG, val);
}

void AppSettings::on_spinBox_animationTime_valueChanged(int val)
{
   this->settings.setValue(ANIMATION_TIME_TAG, val);
}

void AppSettings::on_checkBox_animationEnabled_stateChanged(int state)
{
   bool boolState = state == Qt::Checked ? true : false;
   this->settings.setValue(ANIMATION_ENABLED_TAG, boolState);
   this->ui->spinBox_animationTime->setEnabled(boolState);
}

void AppSettings::on_checkBox_visualize_stateChanged(int state)
{
   bool boolState = state == Qt::Checked ? true : false;
   this->settings.setValue(VISUALIZE_TAG, boolState);
   this->ui->spinBox_animationTime->setEnabled(boolState);
   this->ui->checkBox_animationEnabled->setEnabled(boolState);
}

void AppSettings::on_spinBox_antSize_valueChanged(int val)
{
   this->settings.setValue(ANT_SIZE_TAG, val);
}

void AppSettings::on_pushButton_selectAntColor_clicked()
{
   QColor color = QColorDialog::getColor(getAntsColor(), this);
   if(color.isValid())
   {
      this->settings.setValue(ANT_COLOR_TAG, color);
   }
}

void AppSettings::on_spinBox_constNumberOfAnts_valueChanged(int val)
{
   this->settings.setValue(ANT_CONST_NUMBER_TAG, val);
}

void AppSettings::on_checkBox_isConstNumberOfAnts_stateChanged(int state)
{
   bool boolState = state == Qt::Checked ? true : false;
   this->settings.setValue(ANT_CONST_NUMBER_ENABLED_TAG, boolState);
}

void AppSettings::on_spinBox_antsAnimationTime_valueChanged(int val)
{
   this->settings.setValue(ANT_ANIMATION_TIME_TAG, val);
}

void AppSettings::on_radioButton_noStartingMaze_clicked()
{
   this->settings.setValue(STARTING_MAZE_TAG, static_cast<int>(StartingMaze::NO_STARTING));
}

void AppSettings::on_radioButton_loadMazeFromFileOnStart_clicked()
{
   this->settings.setValue(STARTING_MAZE_TAG, static_cast<int>(StartingMaze::LOAD_FROM_FILE));
}

void AppSettings::on_radioButton_generateMazeOnStart_clicked()
{
   this->settings.setValue(STARTING_MAZE_TAG, static_cast<int>(StartingMaze::GENERATE_NEW));
}
