#pragma once

#include <QIODevice>

class SpeechToTextPlugin : public QIODevice
{
    Q_OBJECT
    Q_PROPERTY(QString wakeWord READ wakeWord WRITE setWakeWord NOTIFY wakeWordChanged)
    Q_PROPERTY(bool asking READ isAsking WRITE setAsking NOTIFY askingChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

public:
    explicit SpeechToTextPlugin(QObject *parent = nullptr){};
    virtual ~SpeechToTextPlugin() = default;

    virtual QString pluginName() = 0;

    enum State {
        NotStarted = 0,       // Not set up yet
        Running = 1,          // The recognizer is set up and proceeds data
        Paused = 2,           // The microphone input is paused
        NoModelFound = 3,     // No model was found for the system languages
        ModelsMissing = 4,    // The directory where the models are stored is empty
        ErrorWhileLoading = 5 // Unknown error loading the model or recognizer
    };
    Q_ENUM(State);

    virtual void setup(const QString &modelDir, bool *success) = 0;

    // Reimplementation of the writeData method from QIODevice, which takes
    // the incoming audio data and size as arguments and processes them
    // Audio format: PCM 16-Bit mono
    /// qint64 writeData(const char *data, qint64 size) override;

    virtual State state() = 0;
    virtual QString errorString() = 0;

    virtual QString language() = 0;

    virtual QString wakeWord() = 0;
    virtual void setWakeWord(const QString &) = 0;

    // Asking mode. We don't need to wait for the wake word
    virtual bool isAsking() = 0;
    virtual void setAsking(bool) = 0;

    virtual bool hasLookupSupport() = 0;                // Word loopup support
    virtual bool canRecognizeWord(const QString &) = 0; // Can it recognize the word?

Q_SIGNALS:
    // Emit this signal when setup() was succesful
    void loaded();

    void stateChanged();

    // Signal emitted when the wake word is detected in the audio input
    void wakeWordDetected();

    // Emit this signal when the wake word changes!
    void wakeWordChanged();
    void askingChanged(); // And this when the asking mode changes

    // Signal emitted when the recognizer has finished listening for audio input
    void doneListening();

    // Signal emitted when the recognizer has recognized some text in the audio input
    void textUpdated(const QString &text);

    void answerReady(QString);

private:
    inline qint64 readData(char *data, qint64 size) final { return size; };
};

#define SpeechToText_iid "voiceassistant.speechtotext/1.0"
Q_DECLARE_INTERFACE(SpeechToTextPlugin, SpeechToText_iid)
