# Introduction
This library provides a standard and secure communication channel between two devices mainly via UART (although other protocols can be used). 
It is composed of multiple modules that can be used independently from each other. So a more specific implementation can be used for any step.

## Modules
### Serialization
The first step of the communication is to serialize the data structures and messages. This messages can be defined via classes that inherit from protocol::serial::Serializable or protocol::messages::ProtocolMessage.
#### Basic serialization example (protocol::serial::Serializable)
```
class ExampleSerializable : public protocol::serial::Serializable {
    public:
        virtual void serialize(serial::Stream& stream) const override;
        virtual void deserialize(serial::Stream& stream) override;

    private:
        // Serializable variables definition
        protocol::primitives::String name;
        protocol::primitives::Int8 value;

        // Non serializable variables
        int index;

        // Serialization implementation methods
}

void ExampleSerializable::serialize(serial::Stream& stream) const {
    stream << name << value;
    ++index; // Other logic
}

void ExampleSerializable::deserialize(serial::Stream& stream) {
    stream >> name >> value; // Deserialize into inner variables
    --index; // Other logic
}
```
Using the primitives simplify the serialization and deserialization process for the creation of new message types. 
> **Important**
>
> This messages do not carry any type of identification, so incorrect reading is possible. If identification is needed the use of the class protocol::messages::ProtocolMessage is recommended.

#### Protocol message example
```
class ExampleMessage : public protocol::messages::ProtocolMessage {
    public:
        // Serializable variables
        protocol::primitives::Int32 hash;
        protocol::primitives::Int32 sequence;

        virtual void serialize(serial::Stream& stream) const override;
        virtual void deserialize(serial::Stream& stream) override;
        virtual uint8_t type() const;

    private: 
    // Private implementation
}

void ExampleMessage::serialize(serial::Stream& stream) const {
    stream << sequence << hash;
}

void ExampleMessage::deserialize(serial::Stream& stream) {
    stream >> sequence >> hash; // Deserialize into inner variables
}

uint8_t ExampleMessage::type() {
        return protocol::messages::Types::EXAMPLE_MESSAGE;
}
```
---
For a more complete explanation of the process of creating a message [click here](@ref ./docs/markdown/serialization.md)

### Base128 encoding and decoding
This protocol uses Base128 to encode and decode the messages as this allows better control over the communication channel (such as sending EOF and control characters over the line).
The implementation of this encoding and decoding is done via a protocol::serial::Stream implementation. So it's available for more than this purpose.
#### Using the protocol::
