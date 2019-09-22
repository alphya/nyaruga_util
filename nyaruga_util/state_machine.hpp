//              Copyright (c) 2019 alphya
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/*
 *  Copyright (c) 2011, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of 16b.it nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef NYARUGA_UTIL_STATE_MACHINE_HPP
#   define NYARUGA_UTIL_STATE_MACHINE_HPP

#   pragma once

#   include <boost/bind.hpp>
#   include <boost/signals2/signal.hpp>
#   include <map>
#   include <memory>
#   include <nyaruga_util/pointer_wrapper.hpp>
#   include <string>

namespace nyaruga::util {

namespace detail {
struct empty_data {
};
} // namespace detail

namespace hide_name_from_adl {

template <typename SharedData = detail::empty_data>
class state {
public:
   using label_type = std::string;

private:
   pointer_wrapper<SharedData> m_data_ptr;

public:
   boost::signals2::signal<void(std::string)> change_state;

   virtual ~state() noexcept = default;

   auto & get_shared_data() noexcept { return *m_data_ptr; }
   void set_shared_data(const pointer_wrapper<SharedData> & shared_data) noexcept { m_data_ptr = shared_data; }

   // Give each state a unique name
   [[nodiscard]] virtual label_type get_label() const noexcept = 0;

   virtual void state_enter(){};
   virtual void state_exit(){};
   virtual void initialize(){};
   virtual void update(){};
   virtual void finalize(){};
};

namespace detail {
// shared_ptr に渡すためのカスタムデリータ
// MSVC のコンパイラでは直接渡せないようなので、ここで定義
constexpr inline auto my_deleter = [](auto * s) { s->finalize(); delete s; };
} // namespace detail

template <typename SharedData = detail::empty_data>
class state_machine {
public:
   using state_ptr = std::shared_ptr<state<SharedData>>;
   using state_itr = typename std::map<std::string, state_ptr>::iterator;

private:
   SharedData m_data;
   state_ptr m_current_state;
   std::map<std::string, state_ptr> m_states;

public:
   template <class T>
   state_ptr add_state()
   {
      state_ptr state(new T, detail::my_deleter);
      return add_state(state);
   }

   state_ptr add_state(state_ptr state)
   {
      state->set_shared_data(&m_data);
      state->initialize();
      state->change_state.connect(boost::bind(&state_machine::change_state, this, _1));
      m_states.insert(std::make_pair(state->get_label(), state));
      return state;
   }

   SharedData & getSharedData() noexcept
   {
      return m_data;
   }

   std::map<std::string, state_ptr> getStates() const noexcept
   {
      return m_states;
   }

   bool change_state(const std::string & label)
   {
      state_itr it = m_states.find(label);

      if (it == m_states.end())
         return false;

      else if (it->second != m_current_state) {
         if (m_current_state) m_current_state->state_exit();
         m_current_state = it->second;
         m_current_state->state_enter();
         return true;
      }

      else
         return false;
   }

   void update()
   {
      if (m_current_state) m_current_state->update();
   }
};

} // namespace hide_name_from_adl

using namespace hide_name_from_adl;

} // namespace nyaruga::util

#endif //NYARUGA_UTIL_STATE_MACHINE_HPP

/* usage


#include "state_machine.hpp"
#include <iostream>

using namespace nyaruga::util;

struct my_data
{
   std::string data;
};

class my_state1 : public state<my_data>
{
public:
   using state::state;
   void update() override
   {
      state::get_shared_data().data = "from my_state1 to my_state2 message.";
      change_state("my_state2");
   }

   [[nodiscard]]
   label_type get_label() const noexcept override { return "my_state1"; }

};

class my_state2 : public state<my_data>
{
public:
   using state::state;
   void update() override
   {
      std::cout << "Hello! from my_state2";
   }

   [[nodiscard]]
   label_type get_label() const noexcept override { return "my_state2"; }

};


int main()
{
   state_machine<my_data> machine;
   machine.add_state<my_state1>();
   machine.add_state<my_state2>();

   machine.change_state("my_state1");
   machine.update();
   machine.update();

   std::cout << machine.getSharedData().data;

}


*/
