/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License ("the license") as
 * published by the Free Software Foundation, either version 3 of the License,
 * or any later version.
 *
 * In accordance with Section 7(e) of the license, the licensing of the Program
 * under the license does not imply a trademark license. Therefore any rights,
 * title and interest in our trademarks remain entirely with us.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you develop
 * commercial activities involving this program without disclosing the source
 * code of your own applications
 */
#ifndef _STX_STATS_STATSDAGENT_H_
#define _STX_STATS_STATSDAGENT_H_
#include <thread>
#include "eventql/util/stdtypes.h"
#include "eventql/util/duration.h"
#include "eventql/util/exception.h"
#include "eventql/util/net/inetaddr.h"
#include "eventql/util/net/udpsocket.h"
#include "eventql/util/stats/stat.h"
#include "eventql/util/stats/statsrepository.h"

namespace stats {

class StatsdAgent {
public:
  static const size_t kMaxPacketSize = 1024 * 48; // 48k

  StatsdAgent(
      InetAddr addr,
      Duration report_interval);

  StatsdAgent(
      InetAddr addr,
      Duration report_interval,
      StatsRepository* stats_repo);

  ~StatsdAgent();

  void start();
  void stop();

protected:
  void report();
  void reportValue(const String& path, Stat* stat, Vector<String>* out);
  void reportDelta(const String& path, Stat* stat, Vector<String>* out);

  void sendToStatsd(const Vector<String>& lines);
  void sendToStatsd(const Buffer& packet);

  net::UDPSocket sock_;
  InetAddr addr_;
  std::atomic<bool> running_;
  std::thread thread_;
  StatsRepository* stats_repo_;
  Duration report_interval_;

  HashMap<String, double> last_values_;
};


}
#endif
