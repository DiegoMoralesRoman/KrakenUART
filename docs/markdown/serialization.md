# Serialization and deserialization
Creating a message is usually done via the implementation of the virtual class protocol::messages::ProtocolMessage because it allows the protocol to identify the message type. If only one type of message was required a simple protocol::serial::Serializable class could be used.

## Standard creation of a protocol message
The creation of a protocol message has to follow these steps.
1. Adding the type to the protocol::messages::Type enum
2. Creating the message class deriving from protocol::messages::ProtocolMessage

## Serialization composition
The main objective of the protocol::serial::Serializable class is to allow an interface for serializable composition. With this more complex serializable data types can be created and sent inside a protocol::messages::ProtocolMessage implementation class.
```
class ComplexSerial : public protocol::serial::Serializable;

class Message : public protocol::messages::ProtocolMessage {
    public:
        ComplexSerial complex;
    ...
}
```
