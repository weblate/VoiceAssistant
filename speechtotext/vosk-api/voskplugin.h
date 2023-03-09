#pragma once

#include "../speechtotextplugin.h"

class VoskPlugin : public SpeechToTextPlugin
{
    Q_OBJECT
public:
    explicit VoskPlugin(QObject *parent = nullptr);
    ~VoskPlugin();

    inline QString pluginName() override { return QStringLiteral("vosk"); };

    qint64 writeData(const char *data, qint64 size) override;

    void setup(const QString &modelDir) override;

    inline State state() override { return m_state; };
    inline QString errorString() override { return m_errorString; };
    inline QString language() override { return m_language; };

    inline QString wakeWord() override { return m_wakeWord.left(m_wakeWord.size() - 1); };
    inline void setWakeWord(const QString &wakeWord) override
    {
        m_wakeWord = wakeWord + u' ';
        Q_EMIT wakeWordChanged();
    };

    inline bool isAsking() override { return m_isAsking; };
    inline void setAsking(bool asking) override
    {
        m_isAsking = asking;
        Q_EMIT askingChanged();
    };

private:
    // Private method that takes a JSON string containing the recognized text
    // and parses it to extract the text and emit the appropriate signals
    void parseText(const char *json);

    // Private method that takes a JSON string containing partial recognized text
    // and parses it to extract the text and emit the appropriate signals
    void parsePartial(const char *json);

    void setState(State);

    QString m_wakeWord;
    bool m_isAsking = false;

    State m_state = NotStarted;
    QString m_errorString;

    QString m_language;
};
