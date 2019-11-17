#include "AppSettings.h"
#include "ui_AppSettingsUI.h"



AppSettings::AppSettings(QWidget* parent) :
   QDialog(parent), ui(new Ui::AppSettings)
{
   ui->setupUi(this);

   this->settings.sync();

   //spin boxes
   setStartingValue(MARKER_SIZE_TAG, 10, this->ui->spinBox_markerSize);
   setStartingValue(MAZE_WIDTH_TAG, 10, this->ui->spinBox_mazeWidth);
   setStartingValue(MAZE_HEIGHT_TAG, 10, this->ui->spinBox_mazeHeight);
   setStartingValue(PATH_SIZE_TAG, 3, this->ui->spinBox_pathSize);
   setStartingValue(TILE_SIZE_TAG, 5, this->ui->spinBox_tileSize);
   setStartingValue(ANIMATION_TIME_TAG, 100, this->ui->spinBox_animationTime);
   //checkboxes
   setStartingValue(ANIMATION_ENABLED_TAG, true, this->ui->checkBox_animationEnabled);
   setStartingValue(VISUALIZE_TAG, true, this->ui->checkBox_visualize);


   if(false == this->isAnimationEnabled())
   {
      this->ui->spinBox_animationTime->setEnabled(false);
   }
   if(false == this->isVisualize())
   {
      this->ui->spinBox_animationTime->setEnabled(false);
      this->ui->checkBox_animationEnabled->setEnabled(false);
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

bool AppSettings::isAnimationEnabled() const
{
   return this->settings.value(ANIMATION_ENABLED_TAG).toBool();
}

bool AppSettings::isVisualize() const
{
   return this->settings.value(VISUALIZE_TAG).toBool();
}

void AppSettings::setVisualizeEnabled(bool val)
{
   this->settings.setValue(VISUALIZE_TAG, val);
   this->ui->spinBox_animationTime->setEnabled(val);
   this->ui->checkBox_animationEnabled->setEnabled(val);
}

void AppSettings::setAnimationTime(int val)
{
   this->settings.setValue(ANIMATION_TIME_TAG, val);
}

void AppSettings::setAnimationEnabled(bool val)
{
   this->settings.setValue(ANIMATION_ENABLED_TAG, val);
   this->ui->spinBox_animationTime->setEnabled(val);
}

void AppSettings::setMazeWidth(int val)
{
   this->settings.setValue(MAZE_WIDTH_TAG, val);
}

void AppSettings::setMazeHeight(int val)
{
   this->settings.setValue(MAZE_HEIGHT_TAG, val);
}

void AppSettings::setMarkerSize(int val)
{
   this->settings.setValue(MARKER_SIZE_TAG, val);
}

void AppSettings::setPathSize(int val)
{
   this->settings.setValue(PATH_SIZE_TAG, val);
}

void AppSettings::setTileSize(int val)
{
   this->settings.setValue(TILE_SIZE_TAG, val);
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
