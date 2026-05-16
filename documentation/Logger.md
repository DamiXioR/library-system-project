# Utilities

# Logger

This project implements a **Logger** in modern C++.

---

## Log Message Format

Log messages are formatted as:

```cpp
[LogLevel][LogChannel][DD-MM-YYYY HH:MM:SS.microseconds][file:line] Message
```

Example:
```cpp
[INFO][Book][16-05-2026 14:40:53.663607][./app/main.cpp:777] Hello World!
```

## Log Sinks

The logger uses Log Sinks to determine how and where log messages are processed.

Two log sinks are currently available:

- Console Log Sink
- File Log Sink

Each sink decides independently whether to handle a given log message.


### Log Channels

Log sinks use channels to filter messages.

A sink will only process a log message if its channel matches the message channel.

Additionally, a special wildcard channel "*" can be used.
A sink configured with "*" will accept all channels, as long as the log level requirement is satisfied.

### Log Levels

Each log sink has a minimum log level.
A message is processed only if its level is greater than or equal to the sink’s minimum level.

Supported log levels:
- Debug = 0,
- Info,
- Warning,
- Error,
- Exception

#### Filtering rule:

If a sink has minimum level Info, then:
Debug logs are ignored
Info and higher levels are processed.