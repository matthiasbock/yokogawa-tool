/**
 * @file
 * Instantiate and interact with a Yokogawa WT3000 power analyzer
 */

#ifndef WT3000_HPP
#define WT3000_HPP

#include <string>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <USBInterface.hpp>
#include <Logging.hpp>
#include <vector>

using namespace std;


namespace Yokogawa
{
    namespace WT3000
    {
        namespace USB
        {
            /** Yokogawa USB Vendor ID */
            const uint16_t VID = 0x0b21;

            /** Yokogawa WT3000 USB Product ID */
            const uint16_t PID = 0x0025;

            /**
             * Data from host to device:
             *  URB_BULK via USB endpoint 0x01
             */
            const uint8_t EndpointTransmit = 0x01;

            /**
             * Data from device to host:
             *  URB_BULK via USB endpoint 0x83
             */
            const uint8_t EndpointReceive = 0x83;
        }

        namespace GPIB
        {
            const string ClearStatus = "*CLS";
            const string Identify = "*IDN?";

            namespace Communicate
            {
                const string Group = ":COMMunicate";
                const string Header = ":HEADer";
                const string Overlap = ":OVERlap";
                const string Remote = ":REMote";
                const string Verbose = ":VERBose";
            }

            namespace Input
            {
                const string Group = ":INPut";
                const string Module = ":MODUle";
                const string Voltage = ":VOLTage";
                const string Current = ":CURRent";
            }

            namespace Numeric
            {
                const string Group = ":NUMeric";

                namespace Format
                {
                    const string Group = ":FORMat";
                    const string ASCII = "ASCii";
                    const string Float = "FLOat";

                    typedef enum
                    {
                        FORMAT_FLOAT,
                    } enumFormats;
                }

                const string Value = ":VALue";
            }

            namespace Status
            {
                const string Group = ":STATus";
                const string ExtendedEventStatusEnable = ":EESE";
                const string Filter = ":FILTer";

                namespace Transition
                {
                    const string Rise = "RISE";
                    const string Fall = "FALL";
                    const string Both = "BOTH";
                    const string Never = "NEVER";
                }
            }
        }

        /**
         * A class with abstract methods to interact with a WT3000 power analyzer
         * using the above GPIB commands via a referenced USB interface
         */
        class Interface
        {
        private:
            USBInterface* usb = NULL;
            LogLevel::type loglevel = LogLevel::Debug;

        public:
            Interface();
            Interface(USBInterface* interface)
                :Interface()
            {
                setUSBInterface(interface);
            }
            ~Interface();

            /**
             * Assign a USB interface through which to communicate with the device
             */
            void setUSBInterface(USBInterface* interface)
            {
                usb = interface;
            }

            /**
             * Returns the currently set logging verbosity
             */
            LogLevel::type getLogLevel()
            {
                return loglevel;
            }

            /**
             * Sets the logging verbosity to the desired level
             */
            void setLogLevel(LogLevel::type level)
            {
                loglevel = level;
            }

            /**
             * Convenience method to prepare the device
             * for communication
             */
            void connect();

            void setRemote(bool);

            /**
             * Queries the instrument model
             */
            string identify();

            /**
             * Clears the standard event register, extended event register, and error queue
             */
            void clearStatus();

            void setExtendedEventStatusEnable(bool);

            void setStatusFilter(string number, string condition);

            /**
             * Sets the commands that will operate as overlap commands
             */
            void setOverlap(bool);

            /**
             * Sets whether to return the response to a query using full spelling
             */
            void setVerbose(bool);

            /**
             * Sets whether to add a header to the response to a query
             */
            void setHeader(bool);

            /**
             * Queries the input element type
             */
            string getInputModule(string number);

            void setNumericFormat(string type);
            void setNumericFormat(GPIB::Numeric::Format::enumFormats);

            /**
             * Queries numeric data (measurement results) from the device
             */
            bool getNumericValues(unsigned char* buffer, int length_max, int* length_received);
            vector<float> getNumericValuesAsFloats();
        };
    }
}

#endif
