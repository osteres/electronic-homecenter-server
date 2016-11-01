//
// Created by Thibault PLET on 02/10/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H
#define HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H

#include <action/AbstractPacketAction.h>
#include <com/osteres/automation/transmission/packet/Packet.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/transmission/packet/Command.h>
#include <service/manager/IdentifierManager.h>
#include <iostream>
#include <inttypes.h>

using com::osteres::automation::transmission::packet::Packet;
using action::AbstractPacketAction;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::transmission::packet::Command;
using service::manager::IdentifierManager;


namespace action
{
    namespace packet
    {
        class IdentifierAssignAction : public AbstractPacketAction
        {
        public:

            /**
             * Constructor
             */
            IdentifierAssignAction(
                RepositoryContainer * repositoryContainer,
                Transmitter *transmitter,
                IdentifierManager * identifierManager
            ) : AbstractPacketAction(repositoryContainer)
            {
                this->transmitter = transmitter;
                this->managerIdentifier = identifierManager;
            }

            /**
             * Execute action
             *
             * Send generated identifier to sensor
             */
            bool execute(Packet *packet)
            {
                cout << "Request for a new identifier." << endl;
                // Generate uid
                unsigned char uid = this->managerIdentifier->generateAndStoreFreeUid();
                cout << "Generated identifier: " << (int)uid << endl;

                // Send packet containing identifier
                Packet *p = new Packet();
                p->setTarget(packet->getSourceType()); // Use type for the moment
                p->setSourceType(packet->getTarget()); // Master hasn't identifier
                p->setSourceIdentifier(packet->getTarget());
                p->setCommand(Command::IDENTIFIER_RESPONSE);
                p->setDataUChar1(uid);
                this->transmitter->add(p); // No confirmation needed, else multiple id can be generated for each sensor...
                cout << "Identifier prepared for sending to sensor" << endl;

                this->executed = true;
                return true;
            }

        protected:

            /**
             * Transmitter
             */
            Transmitter * transmitter = NULL;

            /**
             * Manager identifier
             */
            IdentifierManager * managerIdentifier = NULL;
        };
    }
}

#endif //HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H
