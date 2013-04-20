#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <vector>

#include <Core/ResourceManager.h>
#include <Rendering/RenderingManager.h>

struct ChatMessage
{
    std::string mMessage; //the actual message
    int mTimeStamp; //when the message was sent
};

class Chat : public GameRef
{
    public:
        Chat(Game *game, int chatPacketID, std::string userName);
        virtual ~Chat();

        void initialize();
        void update();
        void draw(sf::RenderTarget *target, sf::RenderStates states);

        bool getChatting(){return mChatting;}

        void addMessage(int playerID, std::string message);

    protected:
        sf::Font *mFont;

        std::string mUserName; /// If this is a client, the user name to use
        bool mChatting; /// If it's a client, whether I'm talking or not
        std::string mMessage;

        int mChatPacketID; /// The packet ID for chat messages

        std::vector <ChatMessage> mMessages;
    private:
};

#endif // CHAT_H
