// #include "max485ttl_buffer.hpp"
// #include <Arduino.h>

// RS485_Buffer::RS485_Buffer(uint8_t buffer_size, bool use_end_marker, char end_marker)
// {
//     this->use_end_marker_ = use_end_marker;
//     this->end_marker_ = end_marker;
//     this->buffer_size_ = buffer_size;
//     this->buffer_ = new char[buffer_size];
//     uint8_t buffer_write_cursor = 0;
//     uint8_t buffer_read_cursor = 0;
//     this->buffer_write_cursor_ = buffer_write_cursor;
//     this->buffer_read_cursor_ = buffer_read_cursor;
//     this->data_in_buffer_ = false;
// }

// // Deconstructor
// // delete[] buffer_;

// bool RS485_Buffer::IsDataInBuffer(void)
// {
//     return data_in_buffer_;
// }

// const char *RS485_Buffer::ReadStringBuffer(void)
// {
//     if (data_in_buffer_)
//     {
//         data_in_buffer_ = false;
//         return buffer_;
//     }

//     return "";
// }

// const char *RS485_Buffer::GetBuffer(void)
// {
//     return buffer_;
// }

// char RS485_Buffer::BufferPeek(void)
// {
//     if (BufferAvailable())
//     {
//         return buffer_[buffer_read_cursor_];
//     }
//     return 0;
// }

// char RS485_Buffer::BufferRead(void)
// {
//     if (BufferAvailable())
//     {
//         char c = buffer_[buffer_read_cursor_];
//         if (buffer_read_cursor_ <= buffer_write_cursor_ - 1U)
//         {
//             buffer_read_cursor_ = 0;
//             buffer_write_cursor_ = 0;
//         }

//         return c;
//     }
//     return 0;
// }

// int RS485_Buffer::BufferAvailable(void)
// {
//     return buffer_write_cursor_ - buffer_read_cursor_;
// }

// int RS485_Buffer::ReadIntoBuffer(void)
// {
//     if (!use_buffer_)
//     {
//         return -1;
//     }

//     SetMode(INPUT);
//     int amount_of_chars = 0;

//     while (available() > 0 && data_in_buffer_ == false)
//     {
//         char read_character = read();
//         amount_of_chars++;

//         if (read_character != end_marker_ || !use_end_marker_)
//         {
//             buffer_[buffer_write_cursor_] = read_character;
//             buffer_write_cursor_++;
//             if (buffer_write_cursor_ >= buffer_size_)
//             {
//                 buffer_write_cursor_ = buffer_size_ - 1;
//             }
//         }
//         else
//         {
//             // Check for edge case of \r\n as two characters
//             if (buffer_write_cursor_ > 0)
//             {
//                 if (buffer_[buffer_write_cursor_ - 1] == '\r')
//                 {
//                     buffer_[buffer_write_cursor_ - 1] = '\0'; // terminate the string on \r
//                 }
//             }

//             buffer_[buffer_write_cursor_] = '\0'; // terminate the string
//             buffer_write_cursor_ = 0;
//             data_in_buffer_ = true;
//         }
//     }

//     return amount_of_chars;
// };

// RS485_Buffer RS485_Buffer::operator=(const RS485_Buffer &otherRS485)
// {
//     this->end_marker_ = otherRS485.end_marker_;
//     this->buffer_size_ = otherRS485.buffer_size_;
//     if (this->buffer_)
//     {
//         delete[] this->buffer_;
//     }

//     this->buffer_ = new char[this->buffer_size_];
//     memcpy(this->buffer_, otherRS485.buffer_, buffer_size_);
//     this->buffer_write_cursor_ = otherRS485.buffer_write_cursor_;
//     this->buffer_read_cursor_ = otherRS485.buffer_read_cursor_;
//     this->data_in_buffer_ = otherRS485.data_in_buffer_;
//     return otherRS485;
// }
