#pragma once

#include "main_window.hpp"

#include "character.hpp"
#include "dialog.hpp"

class main_window::shooting: public paint::scene{

#pragma region

	class resource_holder: public paint::resource{
	public:
		paint::solid_brush black_brush, yellow_brush;
		paint::image player_image, enemy1_image;

		resource_holder()
		{
			yellow_brush.set_color(paint::color(255, 255, 0));
			player_image.set_file_name((::get_directory() + L"Player.png").c_str());
			player_image.set_size(paint::size(32, 32));
			enemy1_image.set_file_name((::get_directory() + L"enemy1.png").c_str());
			enemy1_image.set_size(paint::size(30, 30));
		}
		virtual bool create_resource(const paint::creation_params &cp) override
		{
			if(!black_brush.create_resource(cp))
				return false;
			if(!yellow_brush.create_resource(cp))
				return false;
			if(!player_image.create_resource(cp))
				return false;
			if(!enemy1_image.create_resource(cp))
				return false;
			return true;
		}
		virtual void destroy_resource() override
		{
			black_brush.destroy_resource();
			yellow_brush.destroy_resource();
			player_image.destroy_resource();
			enemy1_image.destroy_resource();
		}
	};
	using resource_type = std::shared_ptr<resource_holder>;

	class bullet: public character{
	public:
		bullet()
		{
			set_radius(5.f);
		}
		void draw(const paint::paint_params &pp, const paint::brush &brush)
		{
			get_circle().fill(pp, brush);
		}
	};

	class player: public character{
		resource_type holder;

	public:
		class bullet: public shooting::bullet{
			bool active = false;
			transform trans = transform::identity();

		public:
			void set_transform(transform t)
			{
				trans = t;
			}
			void SetPosition(paint::point p)
			{
				character::set_position(p);
				if(!get_position().is_inside(paint::rect(-5, -5, 805, 605)))
					set_active(false);
			}
			void move()
			{
				set_vector(trans * get_vector());
				character::move();
				if(!get_position().is_inside(paint::rect(-5, -5, 805, 605)))
					set_active(false);
			}
			operator bool() const
			{
				return active;
			}
			void set_active(bool a=true)
			{
				active = a;
			}
		};

		player(const resource_type &h): holder(h)
		{
			this->set_size(paint::size(32, 32));
			this->set_radius(4.f);
		}
		~player()
		{
		}

		void draw(const paint::paint_params &pp)
		{
			auto &image = holder->player_image;
			image.set_position(get_position());
			image.draw(pp);
			get_circle().draw(pp, holder->black_brush);
		}
	};

	class enemy: public character{
	protected:
		resource_type holder;

		int hitpoint;
		std::chrono::system_clock::time_point time;

		std::mt19937_64 rand{static_cast<std::mt19937_64::result_type>(std::time(nullptr))};

	public:
		class bullet: public shooting::bullet{
		protected:
			resource_type holder;

		public:
			bullet(const resource_type &h): holder(h)
			{
			}
			virtual ~bullet()
			{
			}
			virtual void move(const player &) = 0;
			virtual void draw(const paint::paint_params &pp) = 0;
		};
		using bullets_type = std::vector<bullet*>;

		enemy(const resource_type &h): holder(h)
		{
		}
		virtual ~enemy()
		{
		}
		void set_hitpoint(int hp)
		{
			hitpoint = hp;
		}
		int get_hitpoint() const
		{
			return hitpoint;
		}

		virtual void move(const player &, bullets_type &) = 0;
		virtual void draw(const paint::paint_params &) = 0;
	};
	
	class enemy1: public enemy{
		class bullet: public enemy::bullet{
		public:
			bullet(const resource_type &h): enemy::bullet(h)
			{
			}
			void move(const player &) override
			{
				character::move();
			}
			void draw(const paint::paint_params &pp) override
			{
				get_circle().fill(pp, holder->black_brush);
			}
		};

	public:
		enemy1(const resource_type &h): enemy(h)
		{
			set_radius(15.f);
			set_size(paint::size(30, 30));
			time = std::chrono::system_clock::now();
		}
		~enemy1()
		{
		}

		void move(const player &player, bullets_type &bullets) override
		{
			auto now = std::chrono::system_clock::now();

			auto v = player.get_center() - get_center();
			auto rad = std::atan2(v.y, v.x);
			
			set_vector(transform::rotation(rad) * vector(3.f, 0.f));
			character::move();

			if(std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count() >= 500){
				time = now;
				bullets.push_back(new bullet(holder));
				auto &b = bullets.back();
				b->set_position(get_center());
				b->set_vector(transform::rotation(rad) * vector(static_cast<float>(rand() % 4 + 3), 0.f));
			}
		}
		void draw(const paint::paint_params &pp) override
		{
			auto &image = holder->enemy1_image;
			image.set_position(get_position());
			image.draw(pp);
		}
	};

#pragma endregion

	main_window &window;

	std::mt19937_64 rand{std::time(nullptr)};

	keyconfig *config;
	std::shared_ptr<resource_holder> holder;

	player player_;

	using bullets_type = std::array<player::bullet, 20>;
	bullets_type bullets;
	std::chrono::system_clock::time_point bullet_time;

	std::vector<enemy*> enemies;
	enemy::bullets_type enemy_bullets;
	std::chrono::system_clock::time_point enemy_time;
	std::chrono::milliseconds enemy_duration;

	bool pause = false;
	int score = 0;

	modal_dialog<2> dialog;

public:
	shooting(main_window *w):
		window(*w),
		config(keyconfig::get_instance()),
		holder(std::make_shared<resource_holder>()),
		player_(holder)
	{
		register_resource(holder.get());

		register_object(&dialog);
		dialog.set_button_text(0, L"はい");
		dialog.set_button_handler(0, [this](){
			this->dialog.hide();
			on_hide();
			on_show();
		});
		dialog.set_button_text(1, L"いいえ");
		dialog.set_button_handler(1, [this](){
			this->dialog.hide();
			this->window.select_scene(main_window::scene::title);
		});
		dialog.set_size(paint::size(300, 200));
		dialog.set_position(paint::point(250, 200));

		add_keyboard_handler([this](unsigned keycode, bool push){
			config->set_keyboard_state(keycode, push);
		}, keycode_range(0, 255));

		add_timer_handler([this](...){
			if(pause)
				return;

			auto state = config->get_state();
			vector v;
			const float velocity = 7.f;

			if(state.up){
				v[1][0] -= velocity;
			}
			if(state.down){
				v[1][0] += velocity;
			}
			if(state.left){
				v[0][0] -= velocity;
			}
			if(state.right){
				v[0][0] += velocity;
			}
			player_.set_vector(v);

			auto now = std::chrono::system_clock::now();

			if(state.b && now - bullet_time >= std::chrono::milliseconds(192)){
				auto it = std::find(bullets.begin(), bullets.end(), false);
				if(it != bullets.end()){
					float r = it->get_radius();
					it->set_position(player_.get_center());
					it->set_vector(vector(0, -10));
					it->set_active();

					auto trans = transform::scale(1.f, 1.01f);
					if(state.a){
						trans = transform::rotation(to_rad(-0.5)) * trans;
					}
					if(state.c){
						trans = transform::rotation(to_rad(0.5)) * trans;
					}
					it->set_transform(trans);
				}

				bullet_time = now;
			}

			if(now - enemy_time >= enemy_duration){
				enemy_duration = std::chrono::milliseconds(this->rand() % 750 + 250);
				enemy_time = now;
				enemies.push_back(new enemy1(holder));
				enemies.back()->set_position(paint::point(static_cast<float>(this->rand() % 800), -50));
			}

			move();

			this->window.repaint();
		}, timer_id);
	}
	~shooting()
	{
		unregister_resource(holder.get());
		unregister_object(&dialog);

		for(auto &enemy: enemies){
			delete enemy;
		}
		for(auto &bullet: enemy_bullets){
			delete bullet;
		}
	}
	virtual void on_show() override
	{
		pause = false;
		score = 0;
		player_.set_position(paint::point(384, 500));
	}
	virtual void on_hide() override
	{
		for(auto &b: bullets){
			b.set_active(false);
		}

		for(auto &b: enemy_bullets){
			delete b;
		}
		enemy_bullets.clear();

		for(auto &e: enemies){
			delete e;
		}
		enemies.clear();
	}
	virtual void on_get_joystick_state(const joystick_id &id, const joystick_state &js) override
	{
		config->set_joystick_state(id, js);
	}
	void gameover()
	{
		pause = true;

		std::wstring message = L"スコア ";
		message += std::to_wstring(score);
		message += L"\nもう一度遊びますか？";
		dialog.set_text(message.c_str());
		dialog.show();
	}
	void move()
	{
		for(auto &b: bullets){
			if(b){
				b.move();
			}
		}

		player_.move();
		auto p = player_.get_position();
		auto r = paint::rect(p, player_.get_size());
		auto s = get_size();
		if(r.left < 0.f)
			p.x = 0.f;
		else if(r.right >= s.width)
			p.x = s.width - 1.f - r.width();
		if(r.top < 0.f)
			p.y = 0.f;
		else if(r.bottom >= s.height)
			p.y = s.height - 1.f - r.height();
		player_.set_position(p);

		auto rect = get_rect();

		for(auto it = enemy_bullets.begin(); it != enemy_bullets.end(); ++it){
			(*it)->move(player_);
			if((*it)->is_colliding(player_)){
				gameover();
				return;
			}

			float r = (*it)->get_radius();
			if(!(*it)->get_center().is_inside(rect - paint::point(r, r) + paint::size(r, r))){
				delete *it;
				*it = nullptr;
			}
		}
		enemy_bullets.erase(std::remove(enemy_bullets.begin(), enemy_bullets.end(), nullptr), enemy_bullets.end());

		for(auto it = enemies.begin(); it != enemies.end(); ++it){
			(*it)->move(player_, enemy_bullets);

			if((*it)->is_colliding(player_)){
				gameover();
				return;
			}

			for(auto &b: bullets){
				if(b && (*it)->is_colliding(b)){
					b.set_active(false);
					delete *it;
					*it = nullptr;
					break;
				}
			}
		}
		auto remover = std::remove(enemies.begin(), enemies.end(), nullptr);
		score += std::distance(remover, enemies.end());
		enemies.erase(remover, enemies.end());
	}
	virtual void draw(const paint::paint_params &pp) override
	{
		for(auto &b: bullets){
			if(b){
				b.draw(pp, holder->yellow_brush);
			}
		}

		player_.draw(pp);

		for(auto &b: enemy_bullets){
			b->draw(pp);
		}

		for(auto &e: enemies){
			e->draw(pp);
		}

		this->paint::scene::draw(pp);
	}
};
