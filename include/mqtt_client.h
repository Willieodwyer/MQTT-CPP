#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

#include <stddef.h>

#include <mqtt.h>



/**
 * @brief An MQTT client. 
 */
struct mqtt_client {
    /** @brief the socket connected to the broker. */
    int socketfd;

    /** @brief */
    uint16_t pid_lfsr;

    uint16_t keep_alive;

    enum MqttErrors error;

    int response_timeout;
    int number_of_timeouts;
    int number_of_keep_alives;
    double typical_response_time;

    time_t time_of_last_send;

    void (*publish_response_callback)(struct mqtt_response_publish *publish);

    struct {
        uint8_t *mem_start;
        size_t mem_size;
    
        uint8_t *curr;
        size_t curr_sz;
    } recv_buffer;
    struct mqtt_message_queue mq;
};

uint16_t __mqtt_next_pid(struct mqtt_client *client);
ssize_t __mqtt_send(struct mqtt_client *client);
ssize_t __mqtt_recv(struct mqtt_client *client);


ssize_t mqtt_init(struct mqtt_client *client,
                  int sockfd,
                  uint8_t *sendbuf, size_t sendbufsz,
                  uint8_t *recvbuf, size_t recvbufsz,
                  void (*publish_response_callback)(struct mqtt_response_publish *publish));

ssize_t mqtt_connect(struct mqtt_client *client,
                     const char* client_id,
                     const char* will_topic,
                     const char* will_message,
                     const char* user_name,
                     const char* password,
                     uint8_t connect_flags,
                     uint16_t keep_alive);

ssize_t mqtt_publish(struct mqtt_client *client,
                     const char* topic_name,
                     void* application_message,
                     size_t application_message_size,
                     uint8_t publish_flags);

ssize_t __mqtt_puback(struct mqtt_client *client, uint16_t packet_id);
ssize_t __mqtt_pubrec(struct mqtt_client *client, uint16_t packet_id);
ssize_t __mqtt_pubrel(struct mqtt_client *client, uint16_t packet_id);
ssize_t __mqtt_pubcomp(struct mqtt_client *client, uint16_t packet_id);

ssize_t mqtt_subscribe(struct mqtt_client *client,
                       const char* topic_name,
                       int max_qos_level);

ssize_t mqtt_unsubscribe(struct mqtt_client *client,
                         const char* topic_name);

ssize_t mqtt_ping(struct mqtt_client *client);

ssize_t mqtt_disconnect(struct mqtt_client *client);

#endif