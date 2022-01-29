// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2018 Alejandro Sirgo Rica & Contributors

#include "capturelauncher.h"
#include "./ui_capturelauncher.h"
#include "src/core/controller.h"
#include "src/utils/globalvalues.h"
#include "src/utils/screengrabber.h"
#include "src/utils/screenshotsaver.h"
#include "src/widgets/imagelabel.h"
#include <QDrag>
#include <QMimeData>


// https://github.com/KDE/spectacle/blob/941c1a517be82bed25d1254ebd735c29b0d2951c/src/Gui/KSWidget.cpp
// https://github.com/KDE/spectacle/blob/941c1a517be82bed25d1254ebd735c29b0d2951c/src/Gui/KSMainWindow.cpp

CaptureLauncher::CaptureLauncher(QDialog* parent)
  : QDialog(parent)
  , ui(new Ui::CaptureLauncher)
{
    qApp->installEventFilter(this); // see eventFilter()
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(GlobalValues::iconPath()));
    bool ok;

    ui->imagePreview->setScreenshot(ScreenGrabber().grabEntireDesktop(ok));
    ui->imagePreview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);



    show();
}

CaptureLauncher::~CaptureLauncher()
{
    delete ui;
}


/*

CaptureLauncher::CaptureLauncher(QDialog* parent)
  : QDialog(parent)
{

    m_imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_imageLabel,
            &ImageLabel::dragInitiated,
            this,
            &CaptureLauncher::startDrag);




    m_captureType->insertItem(
      1, tr("Rectangular Region"), CaptureRequest::GRAPHICAL_MODE);

#if defined(Q_OS_MACOS)
    // Following to MacOS philosophy (one application cannot be displayed on
    // more than one display)
    m_captureType->insertItem(
      2, tr("Full Screen (Current Display)"), CaptureRequest::FULLSCREEN_MODE);
#else
    m_captureType->insertItem(
      2, tr("Full Screen (All Monitors)"), CaptureRequest::FULLSCREEN_MODE);
#endif


    m_delaySpinBox->setSpecialValueText(tr("No Delay"));

    connect(m_delaySpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            [this](int val) {
                QString sufix = val == 1 ? tr(" second") : tr(" seconds");
                this->m_delaySpinBox->setSuffix(sufix);
            });

    connect(m_launchButton,
            &QPushButton::clicked,
            this,
            &CaptureLauncher::startCapture);
    m_launchButton->setFocus();
;
}

// HACK:
// https://github.com/KDE/spectacle/blob/fa1e780b8bf3df3ac36c410b9ece4ace041f401b/src/Gui/KSMainWindow.cpp#L70
void CaptureLauncher::startCapture()
{
    m_launchButton->setEnabled(false);
    hide();
    auto mode = static_cast<CaptureRequest::CaptureMode>(
      m_captureType->currentData().toInt());
    CaptureRequest req(mode, 600 + m_delaySpinBox->value() * 1000);
    connectCaptureSlots();
    Controller::getInstance()->requestCapture(req);
}

void CaptureLauncher::startDrag()
{
    QDrag* dragHandler = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    mimeData->setImageData(m_imageLabel->pixmap(Qt::ReturnByValue));
#else
    mimeData->setImageData(m_imageLabel->pixmap());
#endif
    dragHandler->setMimeData(mimeData);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    dragHandler->setPixmap(
      m_imageLabel->pixmap(Qt::ReturnByValue)
        .scaled(
          256, 256, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    dragHandler->exec();
#else
    dragHandler->setPixmap(m_imageLabel->pixmap()->scaled(
      256, 256, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    dragHandler->exec();
#endif
}

void CaptureLauncher::connectCaptureSlots()
{
    connect(Controller::getInstance(),
            &Controller::captureTaken,
            this,
            &CaptureLauncher::captureTaken);
    connect(Controller::getInstance(),
            &Controller::captureFailed,
            this,
            &CaptureLauncher::captureFailed);
}

void CaptureLauncher::disconnectCaptureSlots()
{
    // Hack for MacOS
    // for some strange reasons MacOS sends multiple "captureTaken" signals
    // (random number, usually from 1 up to 20).
    // So no it enables signal on "Capture new screenshot" button and disables
    // on first success of fail.
    disconnect(Controller::getInstance(),
               &Controller::captureTaken,
               this,
               &CaptureLauncher::captureTaken);
    disconnect(Controller::getInstance(),
               &Controller::captureFailed,
               this,
               &CaptureLauncher::captureFailed);
}

void CaptureLauncher::captureTaken(QPixmap p, const QRect&)
{
    // MacOS specific, more details in the function disconnectCaptureSlots()
    disconnectCaptureSlots();

    m_imageLabel->setScreenshot(p);
    show();

    auto mode = static_cast<CaptureRequest::CaptureMode>(
      m_captureType->currentData().toInt());

    if (mode == CaptureRequest::FULLSCREEN_MODE) {
        ScreenshotSaver().saveToFilesystemGUI(p);
    }
    m_launchButton->setEnabled(true);
}

void CaptureLauncher::captureFailed()
{
    // MacOS specific, more details in the function disconnectCaptureSlots()
    disconnectCaptureSlots();
    show();
    m_launchButton->setEnabled(true);
}
*/