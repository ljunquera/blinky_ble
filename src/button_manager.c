#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>


#include "button_manager.h"

LOG_MODULE_REGISTER(button_manager, LOG_LEVEL_DBG);




#ifdef CONFIG_GPIO
static struct gpio_dt_spec button_gpio = GPIO_DT_SPEC_GET_OR(
		DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback button_callback;

static void button_isr_callback(const struct device *port,
				struct gpio_callback *cb,
				uint32_t pins)
{
	LOG_DBG("Physical button pressed");
	ARG_UNUSED(port);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);
}
#endif /* CONFIG_GPIO */


int button_manager_init()
{
    #ifdef CONFIG_GPIO
	if (gpio_is_ready_dt(&button_gpio)) {
		int err;

		err = gpio_pin_configure_dt(&button_gpio, GPIO_INPUT);
		if (err) {
			LOG_ERR("failed to configure button gpio: %d", err);
			return err;
		}

		gpio_init_callback(&button_callback, button_isr_callback,
				   BIT(button_gpio.pin));

		err = gpio_add_callback(button_gpio.port, &button_callback);
		if (err) {
			LOG_ERR("failed to add button callback: %d", err);
			return err;
		}

		err = gpio_pin_interrupt_configure_dt(&button_gpio,
						      GPIO_INT_EDGE_TO_ACTIVE);
		if (err) {
			LOG_ERR("failed to enable button callback: %d", err);
			return err;
		}
	}
#endif /* CONFIG_GPIO */
    
    return 0;
}